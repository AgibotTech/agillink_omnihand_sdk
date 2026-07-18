"""Tests for the FastAPI WebSocket server via ASGI TestClient.

No real hand device required — uses an in-process ASGI app.

Run::

    pytest python/test/test_ws_client.py -v

Requires::

    pip install fastapi httpx pytest
"""

import pytest

try:
    from fastapi.testclient import TestClient
    from omnihand_server.app.main import app
    AVAILABLE = True
except ImportError:
    AVAILABLE = False

pytestmark = pytest.mark.skipif(not AVAILABLE, reason="fastapi or omnihand_server not installed")


@pytest.fixture(scope="module")
def client():
    with TestClient(app) as c:
        yield c


class TestWsClientLifecycle:
    def test_connect_disconnect(self, client):
        with client.websocket_connect("/ws"):
            pass

    def test_list_hands_empty(self, client):
        with client.websocket_connect("/ws") as ws:
            ws.send_json({"type": "list", "request_id": 1})
            resp = ws.receive_json()
            assert resp["ok"] is True
            assert resp["result"] == []

    def test_invalid_message_type_returns_error(self, client):
        with client.websocket_connect("/ws") as ws:
            ws.send_json({"type": "nonexistent_type", "request_id": 2})
            resp = ws.receive_json()
            assert resp.get("ok") is False or "error" in resp

    def test_multiple_requests_in_sequence(self, client):
        with client.websocket_connect("/ws") as ws:
            for i in range(1, 4):
                ws.send_json({"type": "list", "request_id": i})
                resp = ws.receive_json()
                assert resp["ok"] is True
                assert resp["request_id"] == i

    def test_multiple_connections(self, client):
        for i in range(1, 4):
            with client.websocket_connect("/ws") as ws:
                ws.send_json({"type": "list", "request_id": i})
                resp = ws.receive_json()
                assert resp["ok"] is True

    def test_describe_methods_by_product_type(self, client):
        with client.websocket_connect("/ws") as ws:
            ws.send_json({"type": "methods", "product_type": "omnihand_2025", "request_id": 10})
            resp = ws.receive_json()
            assert resp["ok"] is True
            result = resp["result"]
            assert isinstance(result.get("methods"), list)
            assert len(result["methods"]) > 0
            assert result.get("product_type") == "omnihand_2025"
            assert result.get("hand_id") is None

    def test_describe_methods_by_product_type_unknown(self, client):
        with client.websocket_connect("/ws") as ws:
            ws.send_json({"type": "methods", "product_type": "nonexistent", "request_id": 11})
            resp = ws.receive_json()
            assert resp.get("ok") is False
