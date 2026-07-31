# OmniHand 服务器接口 API

OmniHand SDK 通过两种网络协议对外暴露所有手部设备：
**WebSocket**（适合实时流式传输和 RPC）与 **REST**（适合简单的请求-响应控制）。
两种协议均使用 JSON 载荷，共享相同的会话模型。

## 架构

```
                 ┌─────────────────────────────────┐
                 │        omnihand 进程             │
                 │  HandService（会话管理）         │
                 │  ┌────────────┐ ┌─────────────┐ │
                 │  │HandWsServer│ │HandRestServer│ │
                 │  └─────┬──────┘ └──────┬───────┘ │
                 └────────┼───────────────┼─────────┘
                          │               │
          ┌───────────────┼───────────────┼───────────────┐
          │               │               │               │
   C++ HandWsClient  Python WS       requests/curl    Web 浏览器
   (websockets 库)   (websockets)    HTTP 客户端     (index.html)
```

单个 `HandService` 可在两个服务器之间共享，这样通过 WebSocket 创建的手部设备
也可以通过 REST 接口进行控制和查询。

## 协议选择

| 协议 | 适用场景 |
|------|----------|
| WebSocket | 实时关节位置流式传输、订阅推送、低延迟 RPC |
| REST | 简单脚本控制、与标准 HTTP 工具链集成、无状态客户端 |

对于单次方法调用，两种协议功能完全等价。

## 快速开始

### 1. 启动服务器

**C++ 独立可执行文件：**
```bash
# 仅 WebSocket
./omnihand_ws_server 127.0.0.1 8765

# 仅 REST
./omnihand_rest_server 127.0.0.1 8000

# 两者共享同一会话（推荐）
./omnihand_server 127.0.0.1 8765 8000
```

**Python（内嵌服务器）：**
```python
from omnihand.websocket import OmniHandWsServer
from omnihand.restful import OmniHandRestServer

ws   = OmniHandWsServer(bind_host="127.0.0.1", port=8765)
rest = OmniHandRestServer(bind_host="127.0.0.1", port=8000,
                           shared_service=ws.service)
ws.start()
rest.start()
input("按 Enter 停止服务器。")
rest.stop()
ws.stop()
```

### 2. 连接客户端

根据使用场景选择合适的客户端类型：

| 客户端 | 运行方式 |
|--------|---------|
| C++ WebSocket 客户端 | `./demo_omnihand_ws_client --host 127.0.0.1 --port 8765 -d zlgcan` |
| C++ REST 客户端 | `./demo_omnihand_rest_client --host 127.0.0.1 --port 8000 -d zlgcan` |
| Python WebSocket 客户端 | `python python/demo/server/demo_ws_client.py --host 127.0.0.1 --port 8765 -d zlgcan` |
| Python REST 客户端 | `python python/demo/server/demo_rest_client.py --host 127.0.0.1 --port 8000 -d zlgcan` |
| Web 浏览器客户端 | 打开 `python/demo/server/index.html`（编辑 `configs/host.toml` 设置服务器地址） |
| curl | `curl http://127.0.0.1:8000/health` |

## 协议文档

- [WebSocket API](API_SERVER_WS.md) — JSON 消息类型、C++ 和 Python 服务器/客户端 API、流式订阅
- [REST API](API_SERVER_REST.md) — HTTP 端点、请求/响应结构、服务器/客户端 API

## 相关文档

- [C++ API 参考](API_CPP.md)
- [Python API 参考](API_PYTHON.md)
