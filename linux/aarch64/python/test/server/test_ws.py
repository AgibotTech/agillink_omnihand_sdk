"""Unit tests for WebSocket request normalization."""

from unittest.mock import MagicMock, patch

from omnihand_server.routers.ws import _process_message


@patch("omnihand_server.routers.ws.get_service")
def test_create_defaults_hand_side(get_service: MagicMock) -> None:
    message = {
        "type": "create",
        "request_id": "default-side",
        "hand_type": "omnihand_2025",
        "conn_method": "zlgcan",
        "conn_config": {},
    }
    get_service.return_value.process_message.return_value = {"ok": True}

    assert _process_message(message, {}) == {"ok": True}
    get_service.return_value.process_message.assert_called_once_with(
        {**message, "hand_side": "left"}
    )


@patch("omnihand_server.routers.ws.get_service")
def test_create_forwards_hand_side(get_service: MagicMock) -> None:
    message = {
        "type": "create",
        "request_id": "right-side",
        "hand_type": "omnihand_2025",
        "hand_side": "right",
        "conn_method": "zlgcan",
        "conn_config": {},
    }
    get_service.return_value.process_message.return_value = {"ok": True}

    assert _process_message(message, {}) == {"ok": True}
    get_service.return_value.process_message.assert_called_once_with(message)
