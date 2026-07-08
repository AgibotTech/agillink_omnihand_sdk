# OmniHand REST API

## Overview

The OmniHand REST server exposes an HTTP/1.1 API at prefix `/v1/hands/`.
All request and response bodies are JSON (`Content-Type: application/json`).
CORS headers are included in every response.

**Default port:** `8000`

See also the OpenAPI specification at `python/docs/openapi.yml`.

---

## C++ Server — `HandRestServer`

### Header

```cpp
#include "omnihand_server/restful/hand_rest_server.h"
using namespace agilink::omnihand::service;
```

### Options

```cpp
struct Options {
    std::string bind_host = "0.0.0.0";
    uint16_t port = 8000;
    HandService* shared_service = nullptr;  // nullptr = own session store
};
```

### API

```cpp
explicit HandRestServer(Options options = {});
~HandRestServer();

void Start();
void Stop();
bool IsRunning() const;

HandService& service();
const HandService& service() const;
```

### Example

```cpp
HandRestServer::Options opts;
opts.bind_host = "127.0.0.1";
opts.port = 8000;

HandRestServer server(opts);
server.Start();
server.Stop();
```

---

## Python Server — `omnihand_server`

Python users can run the FastAPI-based `omnihand_server` package instead of the C++ server:

```bash
pip install fastapi uvicorn
uvicorn omnihand_server.app.main:app --host 0.0.0.0 --port 8000
```

Source: `python/omnihand_server/`

---

## C++ Client — `HandRestClient`

### Header

```cpp
#include "omnihand_server/restful/hand_rest_client.h"
using namespace agilink::omnihand::service;
```

### Options

```cpp
struct Options {
    std::string host = "localhost";
    uint16_t port = 8000;
    int timeout_ms = 10000;
};
```

### API

```cpp
explicit HandRestClient(Options options = {});
~HandRestClient();
// Non-copyable

nlohmann::json GetHealth();
nlohmann::json CreateHand(const nlohmann::json& spec);
nlohmann::json RemoveHand(int hand_id);
nlohmann::json ListHands();
nlohmann::json DescribeHand(int hand_id);
nlohmann::json DescribeMethods(int hand_id);
nlohmann::json DescribeMethodsByProductType(const std::string& product_type);
nlohmann::json CallMethod(int hand_id, const std::string& method,
                           const nlohmann::json& params = {});
```

Status ≥ 400 throws `std::runtime_error` with the error message from the response.

### Example

```cpp
HandRestClient client({"127.0.0.1", 8000});

auto health = client.GetHealth();
// {"status": "ok", "hands": 0}

auto created = client.CreateHand({
    {"product_type", "omnihand_2025"},
    {"conn_method",  "zlgcan"},
    {"hand_side",    "left"},
    {"conn_config",  {{"canfd_device_id", 0}, {"canfd_channel_id", 0}}}
});
int hand_id = created["hand_id"].get<int>();

auto positions = client.CallMethod(hand_id, "get_all_joint_positions");
client.RemoveHand(hand_id);
```

---

## Python Client

```python
import requests

BASE = "http://127.0.0.1:8000"

# Health check
print(requests.get(f"{BASE}/health").json())
# {"status": "ok", "hands": 0}

# Create hand
spec = {
    "product_type": "omnihand_2025",
    "conn_method": "zlgcan",
    "hand_side": "left",
    "conn_config": {"canfd_device_id": 0, "canfd_channel_id": 0},
}
r = requests.post(f"{BASE}/v1/hands", json=spec)
hand_id = r.json()["hand_id"]

# Call method
r = requests.post(f"{BASE}/v1/hands/{hand_id}/methods",
                  params={"method": "get_all_joint_positions"}, json={})
print(r.json())

# Remove hand
requests.delete(f"{BASE}/v1/hands/{hand_id}")
```

Install: `pip install requests`

---

## Endpoint Reference

### Health

| Method | Path | Description |
|--------|------|-------------|
| `GET` | `/health` | Server health and active hand count |

**Response 200:**
```json
{ "status": "ok", "hands": 2 }
```

---

### Hand Sessions — `/v1/hands`

| Method | Path | Description |
|--------|------|-------------|
| `GET` | `/v1/hands` | List all active hand sessions |
| `POST` | `/v1/hands` | Create a new hand session |
| `GET` | `/v1/hands/methods?product_type=<key>` | List methods for a product type (no session required) |
| `GET` | `/v1/hands/{hand_id}` | Get hand details |
| `DELETE` | `/v1/hands/{hand_id}` | Remove a hand session |
| `GET` | `/v1/hands/{hand_id}/methods` | List callable methods |
| `POST` | `/v1/hands/{hand_id}/methods?method=<name>` | Call a method |

#### `POST /v1/hands` — Create hand

Request body:
```json
{
  "product_type": "omnihand_2025",
  "conn_method": "zlgcan",
  "hand_side": "left",
  "conn_config": {
    "canfd_device_id": 0,
    "canfd_channel_id": 0
  }
}
```

`conn_method` values: `zlgcan`, `hcan`, `socketcan`, `rs485`, `usb`, `tj`, `zlgcan_tcp`

For SocketCAN, use `"conn_config": {"can_interface": "can0"}` instead.

For `rs485` or `usb` (`omnihand_2025` only), use `"conn_config": {"uart_port": "/dev/ttyUSB0"}`.

If a session with the same connection parameters already exists, the server returns the existing
session with `"already_exists": true` instead of creating a new one.

Response 201:
```json
{ "hand_id": 1, "product_type": "omnihand_2025" }
```

#### `GET /v1/hands/{hand_id}/methods` — List methods for a session

Response 200:
```json
{
  "methods": [
    { "name": "get_vendor_info",          "category": "info",    "description": "..." },
    { "name": "get_joint_names",          "category": "info",    "description": "..." },
    { "name": "get_all_joint_positions",  "category": "sensing", "description": "..." },
    { "name": "set_all_active_joint_angles", "category": "control", "description": "..." }
  ]
}
```

#### `GET /v1/hands/methods?product_type=<key>` — List methods by product type

Query available methods without a connected device. Valid `product_type` values:
`omnihand_2025`, `omnihand_pro_2025`, `omnihand_dex_umi`, `omnihand_3_lite`, `omnihand_3_ultra_m`.

```bash
curl "http://127.0.0.1:8000/v1/hands/methods?product_type=omnihand_2025"
```

Response 200:
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

Returns 400 for an unknown `product_type`.

#### Available methods

Call `GET /v1/hands/{hand_id}/methods` for a live session, or
`GET /v1/hands/methods?product_type=<key>` to query statically without a device.

| Category | Method | Request body params | Condition |
|----------|--------|---------------------|-----------|
| info | `get_vendor_info` | `{}` | always |
| info | `get_device_info` | `{}` | always |
| info | `get_joint_names` | `{}` | always |
| info | `get_sensor_order` | `{}` | supports tactile |
| position | `get_all_joint_positions` | `{}` | always |
| position | `get_joint_position` | `{joint_motor_index}` | not UMI |
| position | `set_joint_position` | `{joint_motor_index, posi}` | not UMI |
| position | `set_all_joint_positions` | `{vec_posi}` | not UMI |
| position | `get_all_active_joint_angles` | `{}` | not UMI / H3L |
| position | `set_all_active_joint_angles` | `{angles}` | not UMI / H3L |
| velocity | `get_all_joint_velocities` | `{}` | not UMI |
| velocity | `get_joint_velocity` | `{joint_motor_index}` | not UMI |
| velocity | `set_joint_velocity` | `{joint_motor_index, velo}` | not UMI |
| velocity | `set_all_joint_velocities` | `{vec_velo}` | not UMI |
| control_mode | `get_control_mode` | `{joint_motor_index}` | O12 / H3UM |
| control_mode | `set_control_mode` | `{joint_motor_index, mode}` | O12 / H3UM |
| control_mode | `get_all_control_modes` | `{}` | O12 / H3UM |
| control_mode | `set_all_control_modes` | `{ctrl_modes}` | O12 / H3UM |
| mix_ctrl | `mix_control_by_pt` | `{positions, torques}` | O10 / O12 / H3L |
| mix_ctrl | `mix_control_by_pvt` | `{positions, velocities, torques}` | O10 / O12 / H3L |
| gesture | `set_hand_gesture` | `{gesture}` | always |
| gesture | `get_hand_gesture` | `{gesture}` | always |
| current_threshold | `get_current_threshold` | `{joint_motor_index}` | not UMI |
| current_threshold | `set_current_threshold` | `{joint_motor_index, current_threshold}` | not UMI |
| current_threshold | `get_all_current_thresholds` | `{}` | not UMI |
| current_threshold | `set_all_current_thresholds` | `{current_thresholds}` | not UMI |
| temperature | `get_temperature_report` | `{joint_motor_index}` | not UMI |
| temperature | `get_all_temperature_reports` | `{}` | not UMI |
| current | `get_current_report` | `{joint_motor_index}` | not UMI |
| current | `get_all_current_reports` | `{}` | not UMI |
| error | `get_error_report` | `{joint_motor_index}` | not UMI |
| error | `get_all_error_reports` | `{}` | not UMI |
| tactile | `get_tactile_sensor_data` | `{finger_index}` | O10 (1D) |
| tactile | `get_tactile_sensor_data_raw` | `{finger_index}` | O10 / H3L (1D) |
| tactile | `get_all_tactile_sensor_data_raw` | `{}` | O10 / H3L (1D) |
| tactile | `get_tactile_sensor_3d_data` | `{finger_index}` | O12 (3D) |
| communication | `set_request_interval` | `{milliseconds}` | always |
| communication | `get_request_interval` | `{}` | always |
| communication | `set_frame_recv_timeout` | `{milliseconds}` | always |
| communication | `get_frame_recv_timeout` | `{}` | always |
| communication | `show_data_details` | `{show}` | always |
| communication | `set_device_id` | `{device_id}` | always |

> Tactile data can also be pushed in real-time by subscribing to the `"tactile"` stream via WebSocket.

#### `POST /v1/hands/{hand_id}/methods?method=<name>` — Call method

Request body: method parameters as a JSON object (use `{}` for methods with no parameters).

```bash
# Get joint positions (no params)
curl -X POST "http://127.0.0.1:8000/v1/hands/1/methods?method=get_all_joint_positions" \
     -H "Content-Type: application/json" -d "{}"

# Set joint angles
curl -X POST "http://127.0.0.1:8000/v1/hands/1/methods?method=set_all_active_joint_angles" \
     -H "Content-Type: application/json" \
     -d '{"angles": [0.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.0]}'
```

---

## Error Responses

All errors return a JSON body:
```json
{ "detail": "Error message" }
```

| Status | Meaning |
|--------|---------|
| 400 | Invalid request body or parameters |
| 404 | `hand_id` not found |
| 405 | HTTP method not allowed for this path |
| 500 | Internal server error (device communication failure, etc.) |

---

## Demo

### C++ client

```bash
./omnihand_rest_server 127.0.0.1 8000
./demo_omnihand_rest_client --host 127.0.0.1 --port 8000 -d zlgcan
# rs485 / usb (OmniHand 2025 only):
./demo_omnihand_rest_client --host 127.0.0.1 --port 8000 -d rs485 --uart-port /dev/ttyUSB0
./demo_omnihand_rest_client --host 127.0.0.1 --port 8000 -d usb   --uart-port /dev/ttyUSB0
```

Source: `cpp/demo/omnihand_server/demo_rest_client.cc`

### Python client

```bash
pip install requests
python python/demo/server/demo_rest_client.py --host 127.0.0.1 --port 8000 -d zlgcan
# rs485 / usb:
python python/demo/server/demo_rest_client.py --host 127.0.0.1 --port 8000 -d rs485 --uart-port /dev/ttyUSB0
```

Source: `python/demo/server/demo_rest_client.py`

### Web browser client

Open `python/demo/server/index.html`.  
Edit `python/demo/server/configs/host.toml` to point to your server host and port.

### curl

```bash
curl http://127.0.0.1:8000/health
curl http://127.0.0.1:8000/v1/hands
```

---

## Test

### C++ tests (no device required)

```bash
./test_omnihand_server --gtest_filter=*RestClient*
```

Source: `cpp/test/test_omnihand_server.cc`

### Python tests (no device required)

```bash
pip install fastapi httpx pytest
pytest python/test/test_rest_client.py -v
```

Source: `python/test/test_rest_client.py`

---

## Related Documentation

- [Server Interface Overview](API_SERVER.md)
- [WebSocket API](API_SERVER_WS.md)
- [Python API Reference](API_PYTHON.md)
