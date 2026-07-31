"""Schema/model validation and serialization tests."""

import pytest
from pydantic import ValidationError

from omnihand_server.schemas.hand import (
    HandCreateRequest,
    HandCreateResponse,
    HandDetail,
    HandSummary,
    MethodCallResponse,
    MethodListResponse,
)


class TestHandCreateRequest:
    def test_minimal_valid_payload(self) -> None:
        req = HandCreateRequest(hand_type="omnihand_2025", conn_method="zlgcan")
        assert req.hand_type == "omnihand_2025"
        assert req.hand_side == "left"
        assert req.conn_method == "zlgcan"
        assert req.conn_config == {}

    def test_accepts_right_hand_side(self) -> None:
        req = HandCreateRequest(
            hand_type="omnihand_2025",
            hand_side="right",
            conn_method="zlgcan",
        )
        assert req.hand_side == "right"

    def test_rejects_invalid_hand_side(self) -> None:
        with pytest.raises(ValidationError):
            HandCreateRequest(
                hand_type="omnihand_2025",
                hand_side="center",  # type: ignore[arg-type]
                conn_method="zlgcan",
            )

    def test_with_conn_config(self) -> None:
        req = HandCreateRequest(
            hand_type="omnihand_3_lite",
            conn_method="rs485",
            conn_config={"uart_port": "/dev/ttyUSB0", "baudrate": 460800},
        )
        assert req.conn_config["uart_port"] == "/dev/ttyUSB0"

    def test_rejects_invalid_hand_type(self) -> None:
        with pytest.raises(ValidationError):
            HandCreateRequest(hand_type="robot_hand_x99", conn_method="zlgcan")

    def test_rejects_invalid_conn_method(self) -> None:
        with pytest.raises(ValidationError):
            HandCreateRequest(hand_type="omnihand_2025", conn_method="bluetooth")

    def test_rejects_missing_hand_type(self) -> None:
        with pytest.raises(ValidationError):
            HandCreateRequest(conn_method="zlgcan")  # type: ignore[call-arg]

    def test_rejects_missing_conn_method(self) -> None:
        with pytest.raises(ValidationError):
            HandCreateRequest(hand_type="omnihand_2025")  # type: ignore[call-arg]

    def test_rejects_wrong_type_for_config(self) -> None:
        with pytest.raises(ValidationError):
            HandCreateRequest(
                hand_type="omnihand_2025",
                conn_method="zlgcan",
                conn_config="not_a_dict",  # type: ignore[arg-type]
            )

    def test_all_valid_hand_types_accepted(self) -> None:
        valid_types = [
            "omnihand_2025",
            "omnihand_pro_2025",
            "omnihand_dex_umi",
            "omnihand_3_lite",
            "omnihand_3_ultra_m",
        ]
        for ht in valid_types:
            req = HandCreateRequest(hand_type=ht, conn_method="zlgcan")
            assert req.hand_type == ht

    def test_all_valid_conn_methods_accepted(self) -> None:
        valid_methods = [
            "zlgcan", "hcan", "socketcan", "rs485", "usb", "zlgcan_tcp",
        ]
        for cm in valid_methods:
            req = HandCreateRequest(hand_type="omnihand_2025", conn_method=cm)
            assert req.conn_method == cm


class TestHandCreateResponse:
    def test_new_hand_response(self) -> None:
        resp = HandCreateResponse(hand_id=1, already_created=False)
        assert resp.hand_id == 1
        assert resp.already_created is False

    def test_idempotent_response(self) -> None:
        resp = HandCreateResponse(hand_id=5, already_created=True)
        assert resp.hand_id == 5
        assert resp.already_created is True

    def test_model_dump(self) -> None:
        resp = HandCreateResponse(hand_id=3, already_created=False)
        assert resp.model_dump() == {"hand_id": 3, "already_created": False}


class TestHandSummary:
    def test_from_dict(self) -> None:
        s = HandSummary(hand_id=1, hand_type="omnihand_3_lite", conn_method="usb")
        assert s.hand_id == 1
        assert s.hand_type == "omnihand_3_lite"
        assert s.conn_method == "usb"


class TestHandDetail:
    def test_minimal_fields(self) -> None:
        d = HandDetail(hand_type="omnihand_2025", conn_method="zlgcan")
        assert d.device_info is None
        assert d.vendor_info is None

    def test_with_device_and_vendor_info(self) -> None:
        d = HandDetail(
            hand_type="omnihand_2025",
            conn_method="zlgcan",
            device_info={
                "hand_device_id": 1,
                "commu_params": {
                    "bitrate": 500000,
                    "sample_point": 80,
                    "dbitrate": 2000000,
                    "dsample_point": 75,
                },
            },  # type: ignore[arg-type]
            vendor_info={
                "product_model": "X",
                "product_seq_num": "Y",
                "hardware_version": {"major": 1, "minor": 0, "patch": 0, "res": 0},
                "software_version": {"major": 2, "minor": 1, "patch": 0, "res": 0},
                "voltage": 24,
                "dof": 10,
            },  # type: ignore[arg-type]
        )
        assert d.device_info is not None
        assert d.vendor_info is not None


class TestMethodCallResponse:
    def test_primitive_result(self) -> None:
        resp = MethodCallResponse(result=2048)
        assert resp.result == 2048

    def test_none_result(self) -> None:
        resp = MethodCallResponse(result=None)
        assert resp.result is None

    def test_list_result(self) -> None:
        resp = MethodCallResponse(result=[1, 2, 3])
        assert resp.result == [1, 2, 3]

    def test_dict_result(self) -> None:
        resp = MethodCallResponse(result={"angle": 90})
        assert resp.result == {"angle": 90}

    def test_model_dump(self) -> None:
        resp = MethodCallResponse(result=42)
        assert resp.model_dump() == {"result": 42}


class TestMethodListResponse:
    def test_empty_methods(self) -> None:
        resp = MethodListResponse(product_type="omnihand_2025", methods=[])
        assert resp.methods == []

    def test_with_method_entries(self) -> None:
        from omnihand_server.schemas.hand import MethodInfo

        m = MethodInfo(category="info", name="get_vendor_info")
        resp = MethodListResponse(product_type="omnihand_2025", methods=[m])
        assert len(resp.methods) == 1
        assert resp.methods[0].name == "get_vendor_info"

    def test_model_dump(self) -> None:
        from omnihand_server.schemas.hand import MethodInfo

        m = MethodInfo(category="info", name="init")
        resp = MethodListResponse(product_type="omnihand_2025", methods=[m])
        d = resp.model_dump()
        assert d["product_type"] == "omnihand_2025"
        assert isinstance(d["methods"], list)
        assert d["methods"][0]["name"] == "init"
