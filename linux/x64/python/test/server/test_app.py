"""App-level tests: exception handlers, health endpoint, OpenAPI schema."""

import pytest
from fastapi.testclient import TestClient

from omnihand_server.app.main import create_app

try:
    from omnihand_server.core.service import get_service
    SDK_AVAILABLE = True
except Exception:
    SDK_AVAILABLE = False


@pytest.fixture
def client() -> TestClient:
    app = create_app()
    with TestClient(app) as c:
        yield c


class TestExceptionHandlers:
    def test_value_error_returns_400(self, client: TestClient) -> None:
        @client.app.get("/__test_value_error__")
        def raise_value_error() -> None:
            raise ValueError("test error message")

        r = client.get("/__test_value_error__")
        assert r.status_code == 400
        assert r.json()["detail"] == "test error message"

    def test_index_error_returns_404(self, client: TestClient) -> None:
        @client.app.get("/__test_index_error__")
        def raise_index_error() -> None:
            raise IndexError("not found")

        r = client.get("/__test_index_error__")
        assert r.status_code == 404
        assert "not found" in r.json()["detail"]

    def test_generic_exception_returns_500(self, client: TestClient) -> None:
        @client.app.get("/__test_runtime_error__")
        def raise_runtime_error() -> None:
            raise RuntimeError("sensitive internals")

        r = client.get("/__test_runtime_error__")
        assert r.status_code == 500
        assert r.json()["detail"] == "Internal server error"


class TestHealth:
    def test_returns_ok_with_hands_count(self, client: TestClient) -> None:
        r = client.get("/health")
        assert r.status_code == 200
        data = r.json()
        assert data.get("status") == "ok"
        assert isinstance(data.get("hands"), int)
        assert data["hands"] == 0


class TestOpenAPI:
    def test_schema_title_and_version(self, client: TestClient) -> None:
        r = client.get("/openapi.json")
        assert r.status_code == 200
        schema = r.json()
        assert schema["info"]["title"] == "OmniHand API"
        assert schema["openapi"].startswith("3.")
