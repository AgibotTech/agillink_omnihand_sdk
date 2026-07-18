"""End-to-end REST endpoint tests via FastAPI TestClient (no real device required)."""

from unittest.mock import MagicMock, patch

import pytest
from fastapi.testclient import TestClient

try:
    from omnihand_server.app.main import app
    from omnihand_server.core.sdk import get_supported_conn_types, get_supported_product_types
    AVAILABLE = True
except ImportError:
    AVAILABLE = False

pytestmark = pytest.mark.skipif(not AVAILABLE, reason="omnihand_server not installed")


@pytest.fixture(scope="module")
def client() -> TestClient:
    with TestClient(app) as c:
        yield c


class TestHealth:
    def test_returns_ok_with_hands_count(self, client: TestClient) -> None:
        r = client.get("/health")
        assert r.status_code == 200
        data = r.json()
        assert data.get("status") == "ok"
        assert isinstance(data.get("hands"), int)


class TestGetHands:
    def test_returns_empty_list_initially(self, client: TestClient) -> None:
        r = client.get("/v1/hands/")
        assert r.status_code == 200
        assert r.json() == []


class TestVendorInfo:
    def test_returns_vendor_data(self, client: TestClient) -> None:
        r = client.get("/v1/hands/vendor-info")
        assert r.status_code == 200
        data = r.json()
        assert data["vendor"] == "OmniHand"
        assert "manufacturer" in data


class TestGetHandType:
    def test_product_type_query(self, client: TestClient) -> None:
        r = client.get("/v1/hands/type?query=product_type")
        assert r.status_code == 200
        types = r.json()["type"]
        assert isinstance(types, list)

    def test_conn_type_query(self, client: TestClient) -> None:
        r = client.get("/v1/hands/type?query=conn_type")
        assert r.status_code == 200
        methods = r.json()["type"]
        assert isinstance(methods, list)

    def test_conn_config_query(self, client: TestClient) -> None:
        r = client.get("/v1/hands/type?query=conn_config")
        assert r.status_code == 200
        assert "type" in r.json()

    def test_invalid_query_returns_400(self, client: TestClient) -> None:
        r = client.get("/v1/hands/type?query=invalid")
        assert r.status_code == 400

    def test_missing_query_returns_422(self, client: TestClient) -> None:
        r = client.get("/v1/hands/type")
        assert r.status_code == 422


class TestCreateHand:
    @patch("omnihand_server.routers.hand_router.get_service")
    def test_forwards_hand_side_to_service(self, get_service: MagicMock) -> None:
        from omnihand_server.routers.hand_router import create_hand
        from omnihand_server.schemas.hand import HandCreateRequest

        get_service.return_value.create_hand.return_value = {"hand_id": 1}
        response = create_hand(
            HandCreateRequest(
                hand_type="omnihand_2025",
                hand_side="right",
                conn_method="zlgcan",
            )
        )

        assert response.status_code == 201
        get_service.return_value.create_hand.assert_called_once_with(
            {
                "hand_type": "omnihand_2025",
                "hand_side": "right",
                "conn_method": "zlgcan",
                "conn_config": {},
            }
        )

    def test_invalid_hand_type_returns_422(self, client: TestClient) -> None:
        r = client.post(
            "/v1/hands/",
            json={"hand_type": "invalid_type", "conn_method": "zlgcan", "conn_config": {}},
        )
        assert r.status_code == 422

    def test_missing_required_fields_returns_422(self, client: TestClient) -> None:
        r = client.post("/v1/hands/", json={})
        assert r.status_code == 422

    def test_invalid_json_body_returns_error(self, client: TestClient) -> None:
        r = client.post(
            "/v1/hands/",
            content=b"not json",
            headers={"Content-Type": "application/json"},
        )
        assert r.status_code in (400, 422)


class TestGetHandNotFound:
    def test_returns_404(self, client: TestClient) -> None:
        r = client.get("/v1/hands/9999")
        assert r.status_code == 404

    def test_methods_returns_404(self, client: TestClient) -> None:
        r = client.get("/v1/hands/9999/methods")
        assert r.status_code == 404


class TestReleaseHand:
    def test_nonexistent_returns_404(self, client: TestClient) -> None:
        r = client.delete("/v1/hands/9999")
        assert r.status_code == 404


class TestCallHandMethod:
    def test_missing_method_param_returns_422(self, client: TestClient) -> None:
        r = client.post("/v1/hands/9999/methods", json={})
        assert r.status_code == 422

    def test_hand_not_found_returns_404(self, client: TestClient) -> None:
        r = client.post("/v1/hands/9999/methods?method=get_vendor_info", json={})
        assert r.status_code == 404


class TestRouteOrdering:
    """Static routes (/vendor-info, /type) must not collide with /{hand_id}."""

    def test_vendor_info_not_interpreted_as_hand_id(self, client: TestClient) -> None:
        r = client.get("/v1/hands/vendor-info")
        assert r.status_code == 200
        assert "vendor" in r.json()

    def test_type_not_interpreted_as_hand_id(self, client: TestClient) -> None:
        r = client.get("/v1/hands/type?query=product_type")
        assert r.status_code == 200


class TestOpenAPI:
    def test_schema_is_valid(self, client: TestClient) -> None:
        r = client.get("/openapi.json")
        assert r.status_code == 200
        schema = r.json()
        assert schema["openapi"].startswith("3.")
        assert schema["info"]["title"] == "OmniHand API"
