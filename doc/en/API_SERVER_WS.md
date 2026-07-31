# OmniHand WebSocket API

## Overview

The OmniHand WebSocket server implements [RFC 6455](https://datatracker.ietf.org/doc/html/rfc6455)
over TCP. All messages are UTF-8 JSON. Clients send **masked** frames (required by RFC 6455 for
client→server). The server sends unmasked frames.

Every request includes a `request_id` field. The server echoes it back in the response so the
client can match responses to requests.

**Default port:** `8765`

---

## C++ Server — `HandWsServer`

### Header

```cpp
#include "omnihand_server/websocket/hand_ws_server.h"
using namespace agilink::omnihand::service;
```

### Options

```cpp
struct Options {
    std::string bind_host = "0.0.0.0";
    uint16_t port = 8765;
    int default_publish_interval_ms = 100;
};
```

### API

```cpp
// Constructors
HandWsServer(Options options = {});
HandWsServer(Options options, HandService* shared_service);

void Start();         // Start accepting connections (non-blocking)
void Stop();          // Stop server and join threads
bool IsRunning() const;

HandService& service();         // Access the session store
const HandService& service() const;
```

### Example — standalone

```cpp
HandWsServer::Options opts;
opts.bind_host = "127.0.0.1";
opts.port = 8765;

HandWsServer server(opts);
server.Start();
// ... server runs in background threads ...
server.Stop();
```

### Example — shared session with REST

```cpp
HandWsServer ws_server(ws_opts);
HandRestServer::Options rest_opts;
rest_opts.shared_service = &ws_server.service();
HandRestServer rest_server(rest_opts);

ws_server.Start();
rest_server.Start();
```

---

## Python Server — `omnihand_server`

Python users can run the FastAPI-based `omnihand_server` package, which exposes both WebSocket
and REST on a single port:

```bash
pip install fastapi uvicorn
uvicorn omnihand_server.app.main:app --host 0.0.0.0 --port 8765
```

**WebSocket endpoint:** `ws://host:8765/ws`

Both the C++ and Python servers accept the same message types. Either short names
(`list`, `create`, `remove`, `call`) or long aliases (`list_hands`, `create_hand`,
`remove_hand`, `call_method`) work with both servers.

---

## C++ Client — `HandWsClient`

### Header

```cpp
#include "omnihand_server/websocket/hand_ws_client.h"
using namespace agilink::omnihand::service;
```

### Options

```cpp
struct Options {
    std::string host = "localhost";
    uint16_t port = 8765;
    int connect_timeout_ms = 5000;
    int recv_timeout_ms = 10000;
};
```

### API

```cpp
explicit HandWsClient(Options options = {});
~HandWsClient();
// Non-copyable

void Connect();        // Throws std::runtime_error on failure
void Disconnect();
bool IsConnected() const;

// Low-level: send any JSON request, return full response envelope
nlohmann::json Request(const nlohmann::json& msg);

// High-level helpers (throw on server-side error):
nlohmann::json CreateHand(const nlohmann::json& spec);
nlohmann::json RemoveHand(int hand_id);
nlohmann::json ListHands();
nlohmann::json DescribeMethods(int hand_id);
nlohmann::json DescribeMethodsByProductType(const std::string& product_type);
nlohmann::json CallMethod(int hand_id, const std::string& method,
                           const nlohmann::json& params = {});
```

### Example

```cpp
HandWsClient client({"127.0.0.1", 8765});
client.Connect();

// List hands
auto hands = client.ListHands();  // JSON array

// Create hand
auto created = client.CreateHand({
    {"product_type", "omnihand_2025"},
    {"conn_method",  "zlgcan"},
    {"hand_side",    "left"},
    {"conn_config",  {{"canfd_device_id", 0}, {"canfd_channel_id", 0}}}
});
int hand_id = created["hand_id"].get<int>();

// Call method
auto positions = client.CallMethod(hand_id, "get_all_joint_positions");

client.RemoveHand(hand_id);
client.Disconnect();
```

---

## Python Client

```python
import asyncio, json, websockets

async def demo(host="127.0.0.1", port=8765):
    uri = f"ws://{host}:{port}"
    req_id = 0
    async with websockets.connect(uri) as ws:

        async def request(msg):
            nonlocal req_id
            req_id += 1
            msg["request_id"] = req_id
            await ws.send(json.dumps(msg))
            while True:
                resp = json.loads(await ws.recv())
                if resp.get("request_id") == req_id:
                    return resp

        resp = await request({"type": "list"})
        print(resp["result"])  # []

        resp = await request({
            "type": "create",
            "product_type": "omnihand_2025",
            "conn_method": "zlgcan",
            "hand_side": "left",
            "conn_config": {"canfd_device_id": 0, "canfd_channel_id": 0},
        })
        hand_id = resp["result"]["hand_id"]

        resp = await request({"type": "call", "hand_id": hand_id,
                               "method": "get_all_joint_positions"})
        print(resp["result"])

        await request({"type": "remove", "hand_id": hand_id})

asyncio.run(demo())
```

Install: `pip install websockets`

---

## Message Reference

### Request envelope

```json
{
  "type": "<message type>",
  "request_id": 42,
  ...
}
```

`request_id` may be any JSON value (int, string). The server echoes it back unchanged.

### Response envelope

```json
{
  "type": "result",
  "request_id": 42,
  "ok": true,
  "result": { ... }
}
```

On error:
```json
{
  "type": "result",
  "request_id": 42,
  "ok": false,
  "error": "Human-readable error message"
}
```

### Message types

| `type` | Direction | Description |
|--------|-----------|-------------|
| `list` | C→S | List all registered hand sessions |
| `create` | C→S | Create a new hand session |
| `remove` | C→S | Remove a hand session |
| `describe` | C→S | Get hand details |
| `methods` | C→S | List callable methods for a hand or product type |
| `call` | C→S | Invoke a method on a hand |
| `subscribe` | C→S | Subscribe to a data stream |
| `unsubscribe` | C→S | Unsubscribe from a stream |
| (push) | S→C | Subscription data push (no `request_id`) |

### `create` request

```json
{
  "type": "create",
  "request_id": 1,
  "product_type": "omnihand_2025",
  "conn_method": "zlgcan",
  "hand_side": "left",
  "conn_config": {
    "canfd_device_id": 0,
    "canfd_channel_id": 0
  }
}
```

Response `result`:
```json
{ "hand_id": 1, "product_type": "omnihand_2025" }
```

If a session with the same connection parameters already exists, the server returns the existing
session instead of creating a new one, with an additional `"already_exists": true` field.

For `rs485` or `usb` connections (`omnihand_2025` only), use `uart_port` in `conn_config`:
```json
{
  "type": "create",
  "request_id": 2,
  "product_type": "omnihand_2025",
  "conn_method": "rs485",
  "hand_side": "left",
  "conn_config": { "uart_port": "/dev/ttyUSB0" }
}
```

### `call` request

```json
{
  "type": "call",
  "request_id": 5,
  "hand_id": 1,
  "method": "get_all_joint_positions",
  "params": {}
}
```

### Available methods

Use `{"type": "methods", "hand_id": <id>}` to query the full list for a connected hand, or
`{"type": "methods", "product_type": "<key>"}` to query statically without creating a session.

| Category | Method | Params | Condition |
|----------|--------|--------|-----------|
| info | `get_vendor_info` | — | always |
| info | `get_device_info` | — | always |
| info | `get_joint_names` | — | always |
| info | `get_sensor_order` | — | supports tactile |
| position | `get_all_joint_positions` | — | always |
| position | `get_joint_position` | `joint_motor_index` | not UMI |
| position | `set_joint_position` | `joint_motor_index`, `posi` | not UMI |
| position | `set_all_joint_positions` | `vec_posi` | not UMI |
| position | `get_all_active_joint_angles` | — | not UMI / H3L |
| position | `set_all_active_joint_angles` | `angles` | not UMI / H3L |
| velocity | `get_all_joint_velocities` | — | not UMI |
| velocity | `get_joint_velocity` | `joint_motor_index` | not UMI |
| velocity | `set_joint_velocity` | `joint_motor_index`, `velo` | not UMI |
| velocity | `set_all_joint_velocities` | `vec_velo` | not UMI |
| control_mode | `get_control_mode` | `joint_motor_index` | O12 / H3UM |
| control_mode | `set_control_mode` | `joint_motor_index`, `mode` | O12 / H3UM |
| control_mode | `get_all_control_modes` | — | O12 / H3UM |
| control_mode | `set_all_control_modes` | `ctrl_modes` | O12 / H3UM |
| mix_ctrl | `mix_control_by_pt` | `positions`, `torques` | O10 / O12 / H3L |
| mix_ctrl | `mix_control_by_pvt` | `positions`, `velocities`, `torques` | O10 / O12 / H3L |
| gesture | `set_hand_gesture` | `gesture` | always |
| gesture | `get_hand_gesture` | `gesture` | always |
| current_threshold | `get_current_threshold` | `joint_motor_index` | not UMI |
| current_threshold | `set_current_threshold` | `joint_motor_index`, `current_threshold` | not UMI |
| current_threshold | `get_all_current_thresholds` | — | not UMI |
| current_threshold | `set_all_current_thresholds` | `current_thresholds` | not UMI |
| temperature | `get_temperature_report` | `joint_motor_index` | not UMI |
| temperature | `get_all_temperature_reports` | — | not UMI |
| current | `get_current_report` | `joint_motor_index` | not UMI |
| current | `get_all_current_reports` | — | not UMI |
| error | `get_error_report` | `joint_motor_index` | not UMI |
| error | `get_all_error_reports` | — | not UMI |
| tactile | `get_tactile_sensor_data` | `finger_index` | O10 (1D) |
| tactile | `get_tactile_sensor_data_raw` | `finger_index` | O10 / H3L (1D) |
| tactile | `get_all_tactile_sensor_data_raw` | — | O10 / H3L (1D) |
| tactile | `get_tactile_sensor_3d_data` | `finger_index` | O12 (3D) |
| communication | `set_request_interval` | `milliseconds` | always |
| communication | `get_request_interval` | — | always |
| communication | `set_frame_recv_timeout` | `milliseconds` | always |
| communication | `get_frame_recv_timeout` | — | always |
| communication | `show_data_details` | `show` | always |
| communication | `set_device_id` | `device_id` | always |

`params` may be omitted or set to `{}` for methods that take no arguments.

### `methods` request — by hand session

```json
{ "type": "methods", "request_id": 3, "hand_id": 1 }
```

Response `result` includes `hand_id`, `product_type`, `methods` array, and `streams` array.

### `methods` request — by product type (no session required)

Query available methods for any product type without connecting a physical device:

```json
{ "type": "methods", "request_id": 3, "product_type": "omnihand_2025" }
```

Valid `product_type` values: `omnihand_2025`, `omnihand_pro_2025`, `omnihand_dex_umi`,
`omnihand_3_lite`, `omnihand_3_ultra_m`.

Response `result`:
```json
{
  "hand_id": null,
  "product_type": "omnihand_2025",
  "methods": [
    { "name": "get_vendor_info", "category": "info" },
    { "name": "set_all_joint_positions", "category": "position", "params": ["vec_posi"] }
  ],
  "streams": ["state", "diagnostics", "tactile"]
}
```

### `subscribe` / `unsubscribe`

```json
{
  "type": "subscribe",
  "request_id": 10,
  "hand_id": 1,
  "streams": ["state"]
}
```

Available streams: `state` (joint positions/velocities), `diagnostics` (temperature/current/errors),
`tactile` (tactile sensor data, if supported by the product).

Push messages (no `request_id`, arrive at `publish_interval_ms` rate):
```json
{
  "hand_id": 1,
  "stream": "state",
  "data": { "joint_positions": [0.1, 0.2, ...] }
}
```

---

## Demo

### C++ client

```bash
# Start server first:
./omnihand_ws_server 127.0.0.1 8765

# Run C++ client demo (requires a connected hand device):
./demo_omnihand_ws_client --host 127.0.0.1 --port 8765 -d zlgcan
# rs485 / usb (OmniHand 2025 only):
./demo_omnihand_ws_client --host 127.0.0.1 --port 8765 -d rs485 --uart-port /dev/ttyUSB0
./demo_omnihand_ws_client --host 127.0.0.1 --port 8765 -d usb   --uart-port /dev/ttyUSB0
```

Source: `cpp/demo/omnihand_server/demo_ws_client.cc`

### Python client

```bash
pip install websockets
python python/demo/server/demo_ws_client.py --host 127.0.0.1 --port 8765 -d zlgcan
# rs485 / usb:
python python/demo/server/demo_ws_client.py --host 127.0.0.1 --port 8765 -d rs485 --uart-port /dev/ttyUSB0
```

Source: `python/demo/server/demo_ws_client.py`

### Web browser client

Open `python/demo/server/index.html` in any modern browser.  
Edit `python/demo/server/configs/host.toml` to point to your server address before opening.

---

## Test

### C++ tests (no device required)

```bash
# Lifecycle tests only (start server, connect, list hands, disconnect):
./test_omnihand_server --gtest_filter=*WsClient*

# All WebSocket and REST tests:
./test_omnihand_server --gtest_filter=*Lifecycle*

# Real-device tests via rs485/usb:
./test_omnihand_server -d rs485 --uart-port /dev/ttyUSB0
./test_omnihand_server -d usb   --uart-port /dev/ttyUSB0
```

Source: `cpp/test/test_omnihand_server.cc`

### Python tests (no device required)

```bash
pip install fastapi httpx pytest
pytest python/test/test_ws_client.py -v
```

Source: `python/test/test_ws_client.py`

---

## Related Documentation

- [Server Interface Overview](API_SERVER.md)
- [REST API](API_SERVER_REST.md)
- [C++ API Reference](API_CPP.md)
