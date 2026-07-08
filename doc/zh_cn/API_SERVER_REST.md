# OmniHand REST API

## 概述

OmniHand REST 服务器在 `/v1/hands/` 路径前缀下提供 HTTP/1.1 API 接口。
所有请求和响应体均为 JSON 格式（`Content-Type: application/json`）。
每个响应均包含 CORS 头信息。

**默认端口：** `8000`

完整的接口规范详见 `python/docs/openapi.yml`。

---

## C++ 服务器 — `HandRestServer`

### 头文件

```cpp
#include "omnihand_server/restful/hand_rest_server.h"
using namespace agilink::omnihand::service;
```

### 配置项

```cpp
struct Options {
    std::string bind_host = "0.0.0.0";
    uint16_t port = 8000;
    HandService* shared_service = nullptr;  // nullptr = 使用独立会话管理
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

### 示例

```cpp
HandRestServer::Options opts;
opts.bind_host = "127.0.0.1";
opts.port = 8000;

HandRestServer server(opts);
server.Start();
server.Stop();
```

---

## Python 服务器 — `omnihand_server`

Python 用户可以使用基于 FastAPI 的 `omnihand_server` 包代替 C++ 服务器：

```bash
pip install fastapi uvicorn
uvicorn omnihand_server.app.main:app --host 0.0.0.0 --port 8000
```

源码：`python/omnihand_server/`

---

## C++ 客户端 — `HandRestClient`

### 头文件

```cpp
#include "omnihand_server/restful/hand_rest_client.h"
using namespace agilink::omnihand::service;
```

### 配置项

```cpp
struct Options {
    std::string host = "localhost";
    uint16_t port = 8000;
    int timeout_ms = 10000;  // 请求超时（毫秒）
};
```

### API

```cpp
explicit HandRestClient(Options options = {});
~HandRestClient();
// 禁止拷贝

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

HTTP 状态码 ≥ 400 时抛出 `std::runtime_error`，错误信息来自响应体。

### 示例

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

## Python 客户端

```python
import requests

BASE = "http://127.0.0.1:8000"

# 健康检查
print(requests.get(f"{BASE}/health").json())
# {"status": "ok", "hands": 0}

# 创建手部会话
spec = {
    "product_type": "omnihand_2025",
    "conn_method": "zlgcan",
    "hand_side": "left",
    "conn_config": {"canfd_device_id": 0, "canfd_channel_id": 0},
}
r = requests.post(f"{BASE}/v1/hands", json=spec)
hand_id = r.json()["hand_id"]

# 调用方法
r = requests.post(f"{BASE}/v1/hands/{hand_id}/methods",
                  params={"method": "get_all_joint_positions"}, json={})
print(r.json())

# 移除手部会话
requests.delete(f"{BASE}/v1/hands/{hand_id}")
```

安装依赖：`pip install requests`

---

## 端点参考

### 健康检查

| 方法 | 路径 | 说明 |
|------|------|------|
| `GET` | `/health` | 服务器状态及当前活动手部数量 |

**响应 200：**
```json
{ "status": "ok", "hands": 2 }
```

---

### 手部会话 — `/v1/hands`

| 方法 | 路径 | 说明 |
|------|------|------|
| `GET` | `/v1/hands` | 列出所有活动手部会话 |
| `POST` | `/v1/hands` | 创建新的手部会话 |
| `GET` | `/v1/hands/methods?product_type=<key>` | 按产品类型列出可调用方法（无需会话） |
| `GET` | `/v1/hands/{hand_id}` | 获取指定手部详情 |
| `DELETE` | `/v1/hands/{hand_id}` | 移除手部会话 |
| `GET` | `/v1/hands/{hand_id}/methods` | 列出可调用方法 |
| `POST` | `/v1/hands/{hand_id}/methods?method=<name>` | 调用指定方法 |

#### `POST /v1/hands` — 创建手部会话

请求体：
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

`conn_method` 可选值：`zlgcan`、`hcan`、`socketcan`、`rs485`、`usb`、`tj`、`zlgcan_tcp`

使用 SocketCAN 时，`conn_config` 改为：`{"can_interface": "can0"}`

使用 `rs485` 或 `usb` 时（仅 `omnihand_2025` 支持），`conn_config` 改为：`{"uart_port": "/dev/ttyUSB0"}`

若相同连接参数的会话已存在，服务器直接返回已有会话，并附加 `"already_exists": true` 字段，不会重复创建。

响应 201：
```json
{ "hand_id": 1, "product_type": "omnihand_2025" }
```

#### `GET /v1/hands/{hand_id}/methods` — 列出已连接手部的可调用方法

响应 200：
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

#### `GET /v1/hands/methods?product_type=<key>` — 按产品类型列出方法

无需连接设备，即可查询指定产品类型的可用方法。有效的 `product_type` 值：
`omnihand_2025`、`omnihand_pro_2025`、`omnihand_dex_umi`、`omnihand_3_lite`、`omnihand_3_ultra_m`。

```bash
curl "http://127.0.0.1:8000/v1/hands/methods?product_type=omnihand_2025"
```

响应 200：
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

`product_type` 未知时返回 400。

#### 可调用方法

运行时可通过 `GET /v1/hands/{hand_id}/methods` 获取已连接手部的完整方法列表，
或通过 `GET /v1/hands/methods?product_type=<key>` 在无需设备的情况下静态查询。

| 类别 | 方法名 | 请求体参数 | 适用条件 |
|------|--------|-----------|----------|
| info | `get_vendor_info` | `{}` | 全产品 |
| info | `get_device_info` | `{}` | 全产品 |
| info | `get_joint_names` | `{}` | 全产品 |
| info | `get_sensor_order` | `{}` | 支持触觉传感器 |
| position | `get_all_joint_positions` | `{}` | 全产品 |
| position | `get_joint_position` | `{joint_motor_index}` | 非 UMI |
| position | `set_joint_position` | `{joint_motor_index, posi}` | 非 UMI |
| position | `set_all_joint_positions` | `{vec_posi}` | 非 UMI |
| position | `get_all_active_joint_angles` | `{}` | 非 UMI / H3L |
| position | `set_all_active_joint_angles` | `{angles}` | 非 UMI / H3L |
| velocity | `get_all_joint_velocities` | `{}` | 非 UMI |
| velocity | `get_joint_velocity` | `{joint_motor_index}` | 非 UMI |
| velocity | `set_joint_velocity` | `{joint_motor_index, velo}` | 非 UMI |
| velocity | `set_all_joint_velocities` | `{vec_velo}` | 非 UMI |
| control_mode | `get_control_mode` | `{joint_motor_index}` | O12 / H3UM |
| control_mode | `set_control_mode` | `{joint_motor_index, mode}` | O12 / H3UM |
| control_mode | `get_all_control_modes` | `{}` | O12 / H3UM |
| control_mode | `set_all_control_modes` | `{ctrl_modes}` | O12 / H3UM |
| mix_ctrl | `mix_control_by_pt` | `{positions, torques}` | O10 / O12 / H3L |
| mix_ctrl | `mix_control_by_pvt` | `{positions, velocities, torques}` | O10 / O12 / H3L |
| gesture | `set_hand_gesture` | `{gesture}` | 全产品 |
| gesture | `get_hand_gesture` | `{gesture}` | 全产品 |
| current_threshold | `get_current_threshold` | `{joint_motor_index}` | 非 UMI |
| current_threshold | `set_current_threshold` | `{joint_motor_index, current_threshold}` | 非 UMI |
| current_threshold | `get_all_current_thresholds` | `{}` | 非 UMI |
| current_threshold | `set_all_current_thresholds` | `{current_thresholds}` | 非 UMI |
| temperature | `get_temperature_report` | `{joint_motor_index}` | 非 UMI |
| temperature | `get_all_temperature_reports` | `{}` | 非 UMI |
| current | `get_current_report` | `{joint_motor_index}` | 非 UMI |
| current | `get_all_current_reports` | `{}` | 非 UMI |
| error | `get_error_report` | `{joint_motor_index}` | 非 UMI |
| error | `get_all_error_reports` | `{}` | 非 UMI |
| tactile | `get_tactile_sensor_data` | `{finger_index}` | O10（1D） |
| tactile | `get_tactile_sensor_data_raw` | `{finger_index}` | O10 / H3L（1D） |
| tactile | `get_all_tactile_sensor_data_raw` | `{}` | O10 / H3L（1D） |
| tactile | `get_tactile_sensor_3d_data` | `{finger_index}` | O12（3D） |
| communication | `set_request_interval` | `{milliseconds}` | 全产品 |
| communication | `get_request_interval` | `{}` | 全产品 |
| communication | `set_frame_recv_timeout` | `{milliseconds}` | 全产品 |
| communication | `get_frame_recv_timeout` | `{}` | 全产品 |
| communication | `show_data_details` | `{show}` | 全产品 |
| communication | `set_device_id` | `{device_id}` | 全产品 |

> 触觉数据也支持通过 WebSocket 订阅 `"tactile"` 数据流实时推送。

#### `POST /v1/hands/{hand_id}/methods?method=<name>` — 调用方法

请求体为方法参数（JSON 对象），无参数时使用 `{}`。

```bash
# 获取关节位置（无参数）
curl -X POST "http://127.0.0.1:8000/v1/hands/1/methods?method=get_all_joint_positions" \
     -H "Content-Type: application/json" -d "{}"

# 设置关节角度
curl -X POST "http://127.0.0.1:8000/v1/hands/1/methods?method=set_all_active_joint_angles" \
     -H "Content-Type: application/json" \
     -d '{"angles": [0.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.0]}'
```

---

## 错误响应

所有错误均返回 JSON 格式的响应体：
```json
{ "detail": "错误信息" }
```

| 状态码 | 含义 |
|--------|------|
| 400 | 请求体或参数无效 |
| 404 | `hand_id` 不存在 |
| 405 | 该路径不支持此 HTTP 方法 |
| 500 | 内部服务器错误（设备通信失败等） |

---

## Demo 示例

### C++ 客户端

```bash
./omnihand_rest_server 127.0.0.1 8000
./demo_omnihand_rest_client --host 127.0.0.1 --port 8000 -d zlgcan
# rs485 / usb（仅 OmniHand 2025 支持）：
./demo_omnihand_rest_client --host 127.0.0.1 --port 8000 -d rs485 --uart-port /dev/ttyUSB0
./demo_omnihand_rest_client --host 127.0.0.1 --port 8000 -d usb   --uart-port /dev/ttyUSB0
```

源码：`cpp/demo/omnihand_server/demo_rest_client.cc`

### Python 客户端

```bash
pip install requests
python python/demo/server/demo_rest_client.py --host 127.0.0.1 --port 8000 -d zlgcan
# rs485 / usb：
python python/demo/server/demo_rest_client.py --host 127.0.0.1 --port 8000 -d rs485 --uart-port /dev/ttyUSB0
```

源码：`python/demo/server/demo_rest_client.py`

### Web 浏览器客户端

打开 `python/demo/server/index.html`。  
编辑 `python/demo/server/configs/host.toml`，将服务器地址指向目标主机。

### curl

```bash
curl http://127.0.0.1:8000/health
curl http://127.0.0.1:8000/v1/hands
```

---

## 测试

### C++ 测试（无需实际设备）

```bash
./test_omnihand_server --gtest_filter=*RestClient*
```

源码：`cpp/test/test_omnihand_server.cc`

### Python 测试（无需实际设备）

```bash
pip install fastapi httpx pytest
pytest python/test/test_rest_client.py -v
```

源码：`python/test/test_rest_client.py`

---

## 相关文档

- [服务器接口总览](API_SERVER.md)
- [WebSocket API](API_SERVER_WS.md)
- [Python API 参考](API_PYTHON.md)
