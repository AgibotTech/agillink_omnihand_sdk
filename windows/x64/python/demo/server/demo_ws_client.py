"""WebSocket client demo for OmniHand Server.

Connects to a running OmniHand WebSocket server (C++ Crow binary or Python FastAPI),
creates a hand, calls several methods, then removes the hand.

Usage::

    # Start the C++ server:
    #   omnihand_server --host 127.0.0.1 --port 8000
    # Or start the Python FastAPI server:
    #   uvicorn omnihand_server.app.main:app --port 8000

    python demo_ws_client.py --host 127.0.0.1 --port 8000 -d zlgcan

Requires::

    pip install websockets
"""

import argparse
import asyncio
import json
import sys

try:
    import websockets
except ImportError:
    print("[Error] 'websockets' package not found. Install with: pip install websockets")
    sys.exit(1)


def build_conn_config(device: str, uart_port: str = "") -> dict:
    if device == "socketcan":
        return {"can_interface": "can0"}
    if device in ("rs485", "usb"):
        if not uart_port:
            raise ValueError(f"--uart-port is required for {device}")
        return {"uart_port": uart_port}
    return {"canfd_device_id": 0, "canfd_channel_id": 0}


async def run(host: str, port: int, device: str, uart_port: str = "") -> None:
    uri = f"ws://{host}:{port}/ws"
    print(f"Connecting to {uri} ...")

    async with websockets.connect(uri) as ws:
        print("Connected.\n")
        req_id = 0

        async def request(msg: dict) -> dict:
            nonlocal req_id
            req_id += 1
            msg["request_id"] = req_id
            await ws.send(json.dumps(msg))
            # Wait for response matching our request_id (skip unrelated pushes)
            while True:
                raw = await ws.recv()
                resp = json.loads(raw)
                if resp.get("request_id") == req_id:
                    return resp

        # List hands (should be empty)
        resp = await request({"type": "list"})
        print(f"[list] -> {json.dumps(resp['result'], indent=2)}")

        # Create hand
        spec = {
            "type": "create",
            "hand_type": "omnihand_2025",
            "conn_method": device,
            "conn_config": build_conn_config(device, uart_port),
        }
        print(f"\n[create] request: {json.dumps(spec)}")
        resp = await request(spec)
        if not resp.get("ok"):
            print(f"[create] FAILED: {resp.get('error')}")
            return
        hand_id = resp["result"]["hand_id"]
        print(f"[create] hand_id={hand_id}")

        # Read available methods for this hand
        resp = await request({"type": "methods", "hand_id": hand_id})
        print(f"\n[methods] -> {json.dumps(resp.get('result'), indent=2)}")

        # Get vendor info
        resp = await request({"type": "call", "hand_id": hand_id,
                               "method": "get_vendor_info"})
        print(f"\n[get_vendor_info] -> {json.dumps(resp.get('result'), indent=2)}")

        # Get joint names
        resp = await request({"type": "call", "hand_id": hand_id,
                               "method": "get_joint_names"})
        print(f"[get_joint_names] -> {resp.get('result')}")

        # Get all joint positions
        resp = await request({"type": "call", "hand_id": hand_id,
                               "method": "get_all_joint_positions"})
        print(f"[get_all_joint_positions] -> {resp.get('result')}")

        # Remove hand
        resp = await request({"type": "remove", "hand_id": hand_id})
        print(f"\n[remove] -> {resp.get('result')}")

    print("\nDisconnected.")


def main() -> None:
    parser = argparse.ArgumentParser(description="OmniHand WebSocket client demo")
    parser.add_argument("--host", default="127.0.0.1", help="Server host (default: 127.0.0.1)")
    parser.add_argument("--port", type=int, default=8000, help="Server port (default: 8000)")
    parser.add_argument("-d", "--device", default="zlgcan",
                        choices=["zlgcan", "hcan", "socketcan", "rs485", "usb"],
                        help="Device type (default: zlgcan)")
    parser.add_argument("--uart-port", default="",
                        help="UART device path for rs485/usb (e.g. /dev/ttyUSB0)")
    args = parser.parse_args()

    try:
        asyncio.run(run(args.host, args.port, args.device, args.uart_port))
    except Exception as exc:
        print(f"[Error] {exc}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()
