"""REST client demo for OmniHand Server.

Connects to a running OmniHand REST server (C++ Crow binary or Python FastAPI),
creates a hand, calls several methods, then removes the hand.

Usage::

    # Start the C++ server:
    #   omnihand_server --host 127.0.0.1 --port 8000
    # Or start the Python FastAPI server:
    #   uvicorn omnihand_server.app.main:app --port 8000

    python demo_rest_client.py --host 127.0.0.1 --port 8000 -d zlgcan

Requires::

    pip install requests
"""

import argparse
import json
import sys

try:
    import requests
except ImportError:
    print("[Error] 'requests' package not found. Install with: pip install requests")
    sys.exit(1)


def build_conn_config(device: str, uart_port: str = "") -> dict:
    if device == "socketcan":
        return {"can_interface": "can0"}
    if device in ("rs485", "usb"):
        if not uart_port:
            raise ValueError(f"--uart-port is required for {device}")
        return {"uart_port": uart_port}
    return {"canfd_device_id": 0, "canfd_channel_id": 0}


def run(host: str, port: int, device: str, uart_port: str = "") -> None:
    base = f"http://{host}:{port}"
    print(f"Server: {base}\n")

    # Health check
    r = requests.get(f"{base}/health", timeout=5)
    r.raise_for_status()
    print(f"[GET /health] -> {r.json()}")

    # Query methods by product type (no device required)
    r = requests.get(
        f"{base}/v1/hands/methods",
        params={"product_type": "omnihand_2025"},
        timeout=5,
    )
    r.raise_for_status()
    pt_methods = r.json()
    pt_names = [m["name"] for m in pt_methods.get("methods", [])]
    print(f"[GET /v1/hands/methods] ({len(pt_names)} methods): {pt_names}\n")

    # List hands (should be empty)
    r = requests.get(f"{base}/v1/hands/", timeout=5)
    r.raise_for_status()
    print(f"[GET /v1/hands/] -> {r.json()}")

    # Create hand
    spec = {
        "hand_type": "omnihand_2025",
        "conn_method": device,
        "conn_config": build_conn_config(device, uart_port),
    }
    print(f"\n[POST /v1/hands/] request: {json.dumps(spec)}")
    r = requests.post(f"{base}/v1/hands/", json=spec, timeout=10)
    if r.status_code >= 400:
        print(f"[POST /v1/hands/] FAILED {r.status_code}: {r.text}")
        return
    hand_id = r.json()["hand_id"]
    print(f"[POST /v1/hands/] hand_id={hand_id}")

    # Describe hand
    r = requests.get(f"{base}/v1/hands/{hand_id}", timeout=5)
    r.raise_for_status()
    print(f"\n[GET /v1/hands/{hand_id}] -> {json.dumps(r.json(), indent=2)}")

    # Read available methods for this hand
    r = requests.get(f"{base}/v1/hands/{hand_id}/methods", timeout=5)
    r.raise_for_status()
    print(f"[GET /v1/hands/{hand_id}/methods] -> {json.dumps(r.json(), indent=2)}")

    # Call get_vendor_info
    r = requests.post(
        f"{base}/v1/hands/{hand_id}/methods",
        params={"method": "get_vendor_info"},
        json={},
        timeout=10,
    )
    print(f"\n[POST .../methods get_vendor_info] -> {json.dumps(r.json(), indent=2)}")

    # Call get_joint_names
    r = requests.post(
        f"{base}/v1/hands/{hand_id}/methods",
        params={"method": "get_joint_names"},
        json={},
        timeout=10,
    )
    print(f"[POST .../methods get_joint_names] -> {r.json()}")

    # Call get_all_joint_positions
    r = requests.post(
        f"{base}/v1/hands/{hand_id}/methods",
        params={"method": "get_all_joint_positions"},
        json={},
        timeout=10,
    )
    print(f"[POST .../methods get_all_joint_positions] -> {r.json()}")

    # Remove hand
    r = requests.delete(f"{base}/v1/hands/{hand_id}", timeout=5)
    r.raise_for_status()
    print(f"\n[DELETE /v1/hands/{hand_id}] -> {r.json()}")


def main() -> None:
    parser = argparse.ArgumentParser(description="OmniHand REST client demo")
    parser.add_argument("--host", default="127.0.0.1", help="Server host (default: 127.0.0.1)")
    parser.add_argument("--port", type=int, default=8000, help="Server port (default: 8000)")
    parser.add_argument("-d", "--device", default="zlgcan",
                        choices=["zlgcan", "hcan", "socketcan", "rs485", "usb"],
                        help="Device type (default: zlgcan)")
    parser.add_argument("--uart-port", default="",
                        help="UART device path for rs485/usb (e.g. /dev/ttyUSB0)")
    args = parser.parse_args()

    try:
        run(args.host, args.port, args.device, args.uart_port)
    except Exception as exc:
        print(f"[Error] {exc}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()
