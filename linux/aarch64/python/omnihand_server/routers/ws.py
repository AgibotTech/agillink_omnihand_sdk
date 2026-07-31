"""WebSocket endpoint — command + event protocol for OmniHand SDK.

Client → server messages (JSON):
  {"type":"list",        "request_id":"…"}
  {"type":"create",      "request_id":"…", "hand_type":"…", "hand_side":"left|right", "conn_method":"…", "conn_config":{…}}
  {"type":"remove",      "request_id":"…", "hand_id":1}
  {"type":"describe",    "request_id":"…", "hand_id":1}
  {"type":"methods",     "request_id":"…", "hand_id":1}
  {"type":"call",        "request_id":"…", "hand_id":1, "method":"…", "params":{…}}
  {"type":"subscribe",   "request_id":"…", "hand_id":1, "streams":["state",…]}
  {"type":"unsubscribe", "request_id":"…", "hand_id":1, "streams":["state",…]}

Server → client messages (JSON):
  {"type":"result", "request_id":"…", "ok":true,  "result":…}
  {"type":"result", "request_id":"…", "ok":false, "error":"…"}
  {"type":"event",  "stream":"state", "hand_id":1, "data":…}
"""

import asyncio
import logging
import time
from typing import Any

from fastapi import APIRouter, WebSocket, WebSocketDisconnect

from omnihand_server.core.service import get_service

logger = logging.getLogger(__name__)

ws_router = APIRouter()

_VALID_STREAMS = frozenset({"state", "diagnostics", "tactile"})
_DEFAULT_PUBLISH_INTERVAL = 0.1  # seconds


def _ok(request_id: Any, result: Any) -> dict:
    return {"type": "result", "request_id": request_id, "ok": True, "result": result}


def _err(request_id: Any, error: str) -> dict:
    return {"type": "result", "request_id": request_id, "ok": False, "error": error}


def _process_message(msg: dict, subscriptions: dict[int, set[str]]) -> dict | None:
    request_id = msg.get("request_id")
    msg_type = msg.get("type", "")

    try:
        if msg_type == "subscribe":
            hand_id = int(msg["hand_id"])
            streams = [s for s in msg.get("streams", []) if s in _VALID_STREAMS]
            subscriptions.setdefault(hand_id, set()).update(streams)
            return _ok(request_id, {"hand_id": hand_id, "streams": streams})

        if msg_type == "unsubscribe":
            hand_id = int(msg["hand_id"])
            streams = msg.get("streams", [])
            if hand_id in subscriptions:
                for s in streams:
                    subscriptions[hand_id].discard(s)
                if not subscriptions[hand_id]:
                    del subscriptions[hand_id]
            return _ok(request_id, {"hand_id": hand_id, "streams": streams})

        if msg_type == "create" and msg.get("hand_side") is None:
            msg = {**msg, "hand_side": "left"}

        return get_service().process_message(msg)

    except (KeyError, IndexError) as exc:
        return _err(request_id, f"Missing required field or hand not found: {exc}")
    except (ValueError, TypeError) as exc:
        return _err(request_id, str(exc))
    except Exception as exc:
        logger.exception("Error processing WS message type=%s", msg_type)
        return _err(request_id, str(exc))


@ws_router.websocket("/ws")
async def websocket_endpoint(websocket: WebSocket) -> None:
    await websocket.accept()
    subscriptions: dict[int, set[str]] = {}
    last_publish = time.monotonic()

    try:
        while True:
            try:
                raw = await asyncio.wait_for(websocket.receive_json(), timeout=0.05)
            except asyncio.TimeoutError:
                raw = None
            except WebSocketDisconnect:
                break

            if raw is not None:
                if not isinstance(raw, dict):
                    await websocket.send_json(_err(None, "Expected a JSON object"))
                    continue
                response = _process_message(raw, subscriptions)
                if response is not None:
                    await websocket.send_json(response)

                if raw.get("type") == "subscribe":
                    hand_id = raw.get("hand_id")
                    streams = raw.get("streams", [])
                    if hand_id is not None:
                        for stream in streams:
                            try:
                                event = await asyncio.get_event_loop().run_in_executor(
                                    None, get_service().read_stream, int(hand_id), stream
                                )
                                await websocket.send_json(event)
                            except Exception as exc:
                                await websocket.send_json(_err(None, str(exc)))

            now = time.monotonic()
            if now - last_publish >= _DEFAULT_PUBLISH_INTERVAL:
                last_publish = now
                for hand_id, streams in list(subscriptions.items()):
                    for stream in list(streams):
                        try:
                            event = await asyncio.get_event_loop().run_in_executor(
                                None, get_service().read_stream, hand_id, stream
                            )
                            await websocket.send_json(event)
                        except Exception as exc:
                            await websocket.send_json(
                                {"type": "event", "stream": stream, "hand_id": hand_id,
                                 "error": str(exc)}
                            )

    except WebSocketDisconnect:
        pass
    except Exception:
        logger.exception("Unexpected error in WebSocket handler")
