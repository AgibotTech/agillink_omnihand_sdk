"""Tests for the FastAPI REST server via ASGI TestClient.

No real hand device required — uses an in-process ASGI app.

Run::

    pytest python/test/test_rest_client.py -v

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


class TestRestClientLifecycle:
    def test_health(self, client):
        r = client.get("/health")
        assert r.status_code == 200
        data = r.json()
        assert data.get("status") == "ok"
        assert isinstance(data.get("hands"), int)

    def test_list_hands_empty(self, client):
        r = client.get("/v1/hands")
        assert r.status_code == 200
        assert r.json() == []

    def test_nonexistent_hand_returns_404(self, client):
        r = client.get("/v1/hands/9999")
        assert r.status_code == 404

    def test_nonexistent_hand_methods_returns_404(self, client):
        r = client.get("/v1/hands/9999/methods")
        assert r.status_code == 404

    def test_delete_nonexistent_hand_returns_404(self, client):
        r = client.delete("/v1/hands/9999")
        assert r.status_code == 404

    def test_cors_headers_present(self, client):
        r = client.get("/health", headers={"Origin": "http://localhost:3000"})
        assert r.status_code == 200
        assert "access-control-allow-origin" in r.headers

    def test_create_hand_invalid_body_returns_error(self, client):
        r = client.post(
            "/v1/hands",
            content=b"not json",
            headers={"Content-Type": "application/json"},
        )
        assert r.status_code in (400, 422)

    def test_describe_methods_by_product_type(self, client):
        r = client.get("/v1/hands/methods", params={"product_type": "omnihand_2025"})
        assert r.status_code == 200
        data = r.json()
        assert isinstance(data.get("methods"), list)
        assert len(data["methods"]) > 0
        assert data.get("product_type") == "omnihand_2025"
        assert data.get("hand_id") is None

    def test_describe_methods_by_product_type_unknown(self, client):
        r = client.get("/v1/hands/methods", params={"product_type": "nonexistent"})
        assert r.status_code == 400

    def test_describe_methods_by_product_type_missing_param(self, client):
        r = client.get("/v1/hands/methods")
        assert r.status_code == 422
