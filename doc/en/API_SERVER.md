# OmniHand Server Interface API

The OmniHand SDK exposes every hand device through two network protocols:
**WebSocket** (for real-time streaming and RPC) and **REST** (for simple request-response control).
Both protocols carry JSON payloads and use the same session model.

## Architecture

```
                 ┌─────────────────────────────────┐
                 │        omnihand process          │
                 │  HandService (session store)     │
                 │  ┌────────────┐ ┌─────────────┐ │
                 │  │HandWsServer│ │HandRestServer│ │
                 │  └─────┬──────┘ └──────┬───────┘ │
                 └────────┼───────────────┼─────────┘
                          │               │
          ┌───────────────┼───────────────┼───────────────┐
          │               │               │               │
   C++ HandWsClient  Python WS       requests/curl    Web browser
   (websockets lib)  (websockets)    HTTP client      (index.html)
```

A single `HandService` can be shared between both servers so that a hand created
via WebSocket is also visible and controllable via REST.

## Protocol Selection

| Protocol | Best for |
|----------|----------|
| WebSocket | Real-time joint position streaming, subscriptions, low-latency RPC |
| REST | Simple scripting, integration with standard HTTP tooling, stateless clients |

Both protocols are functionally equivalent for single-shot method calls.

## Quick Start

### 1. Start the server

**C++ binary (standalone):**
```bash
# WebSocket only
./omnihand_ws_server 127.0.0.1 8765

# REST only
./omnihand_rest_server 127.0.0.1 8000

# Both sharing one session store (recommended)
./omnihand_server 127.0.0.1 8765 8000
```

**Python (embedded server):**
```python
from omnihand.websocket import OmniHandWsServer
from omnihand.restful import OmniHandRestServer

ws  = OmniHandWsServer(bind_host="127.0.0.1", port=8765)
rest = OmniHandRestServer(bind_host="127.0.0.1", port=8000,
                           shared_service=ws.service)
ws.start()
rest.start()
input("Press Enter to stop.")
rest.stop()
ws.stop()
```

### 2. Connect a client

Choose the client type that fits your use case:

| Client | How to run |
|--------|-----------|
| C++ WebSocket | `./demo_omnihand_ws_client --host 127.0.0.1 --port 8765 -d zlgcan` |
| C++ REST | `./demo_omnihand_rest_client --host 127.0.0.1 --port 8000 -d zlgcan` |
| Python WebSocket | `python python/demo/server/demo_ws_client.py --host 127.0.0.1 --port 8765 -d zlgcan` |
| Python REST | `python python/demo/server/demo_rest_client.py --host 127.0.0.1 --port 8000 -d zlgcan` |
| Web browser | Open `python/demo/server/index.html` (edit `configs/host.toml` to set host) |
| curl | `curl http://127.0.0.1:8000/health` |

## Protocol Documentation

- [WebSocket API](API_SERVER_WS.md) — JSON message types, server/client C++ and Python APIs, streaming subscriptions
- [REST API](API_SERVER_REST.md) — HTTP endpoints, request/response schemas, server/client APIs

## Related Documentation

- [C++ API Reference](API_CPP.md)
- [Python API Reference](API_PYTHON.md)
