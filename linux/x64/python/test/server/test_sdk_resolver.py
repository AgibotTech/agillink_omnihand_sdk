"""Unit tests for the SDK resolver module (omnihand_server.core.sdk).

These tests run WITHOUT the omnihand C++ extension installed.
"""

from unittest.mock import MagicMock, patch

import pytest

from omnihand_server.core.sdk import (
    get_factory_name,
    get_supported_conn_types,
    get_supported_product_types,
)


class TestGetSupportedProductTypes:
    def test_returns_five_types(self) -> None:
        # Without SDK loaded, get_supported_product_types falls back to empty
        # (no SDK = no _hand_classes). We just check it returns a list.
        types = get_supported_product_types()
        assert isinstance(types, list)
        for t in types:
            assert t in (
                "omnihand_2025",
                "omnihand_pro_2025",
                "omnihand_dex_umi",
                "omnihand_3_lite",
                "omnihand_3_ultra_m",
            )


class TestGetSupportedConnTypes:
    def test_returns_expected_methods(self) -> None:
        methods = get_supported_conn_types()
        assert isinstance(methods, list)
        for m in methods:
            assert m in ("zlgcan", "hcan", "socketcan", "rs485", "usb", "zlgcan_tcp")

    def test_contains_core_methods(self) -> None:
        methods = get_supported_conn_types()
        assert "zlgcan" in methods
        assert "hcan" in methods


class TestGetFactoryName:
    @pytest.mark.parametrize(
        "conn_method,expected",
        [
            ("zlgcan", "create_hand_by_zlgcan"),
            ("hcan", "create_hand_by_hcan"),
            ("socketcan", "create_hand_socketcan"),
            ("rs485", "create_hand_by_rs485"),
            ("tj", "create_hand_by_tj"),
            ("zlgcan_tcp", "create_hand_by_zlgcan_tcp"),
        ],
    )
    @patch("omnihand_server.core.sdk._ensure_sdk")
    def test_maps_correctly(
        self, mock_ensure: MagicMock, conn_method: str, expected: str
    ) -> None:
        from omnihand_server.core.sdk import _hand_classes

        mock_cls = MagicMock(spec=[expected])
        _hand_classes["omnihand_2025"] = mock_cls
        assert get_factory_name("omnihand_2025", conn_method) == expected

    def test_raises_for_unknown_method(self) -> None:
        with pytest.raises(ValueError, match="Unknown connection method"):
            get_factory_name("omnihand_2025", "bluetooth")


class TestGetHandClass:
    @patch("omnihand_server.core.sdk._ensure_sdk")
    def test_returns_class_for_valid_type(self, mock_ensure: MagicMock) -> None:
        from omnihand_server.core.sdk import _hand_classes, get_hand_class

        fake_cls = MagicMock()
        _hand_classes["omnihand_2025"] = fake_cls

        result = get_hand_class("omnihand_2025")
        assert result is fake_cls

    @patch("omnihand_server.core.sdk._ensure_sdk")
    def test_raises_for_unknown_type(self, mock_ensure: MagicMock) -> None:
        from omnihand_server.core.sdk import get_hand_class

        with pytest.raises(ValueError, match="Unknown hand type"):
            get_hand_class("robot_hand_x99")


class TestGetProductType:
    @patch("omnihand_server.core.sdk._ensure_sdk")
    def test_returns_int_for_valid_type(self, mock_ensure: MagicMock) -> None:
        from omnihand_server.core.sdk import _product_types, get_product_type

        _product_types["omnihand_3_lite"] = 3
        assert get_product_type("omnihand_3_lite") == 3

    @patch("omnihand_server.core.sdk._ensure_sdk")
    def test_raises_for_unknown_type(self, mock_ensure: MagicMock) -> None:
        from omnihand_server.core.sdk import get_product_type

        with pytest.raises(ValueError, match="Unknown hand type"):
            get_product_type("invalid")
