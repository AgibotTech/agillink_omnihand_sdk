from typing import Any

from fastapi import APIRouter, Body, Query
from fastapi.responses import JSONResponse

from omnihand_server.core.sdk import (
    get_supported_conn_configs,
    get_supported_conn_types,
    get_supported_product_types,
)
from omnihand_server.core.service import get_service
from omnihand_server.schemas.hand import HandCreateRequest

import logging
import toml
from pathlib import Path

logger = logging.getLogger(__name__)

_VENDOR_TOML = Path(__file__).resolve().parent.parent / "configs" / "vendor.toml"
try:
    _VENDOR_INFO: dict[str, str] = toml.loads(_VENDOR_TOML.read_text())
except (FileNotFoundError, toml.TomlDecodeError) as exc:
    logger.warning("Failed to load vendor.toml: %s. Using defaults.", exc)
    _VENDOR_INFO = {
        "vendor": "OmniHand",
        "manufacturer": "Unknown",
        "contact": "",
        "license": "Proprietary",
        "sdk_version": "unknown",
    }

hands_router = APIRouter(tags=["hands"])


@hands_router.get("/")
def get_hands() -> Any:
    return get_service().list_hands()


@hands_router.post("/")
def create_hand(hand: HandCreateRequest) -> JSONResponse:
    spec = {
        "hand_type": hand.hand_type,
        "hand_side": hand.hand_side,
        "conn_method": hand.conn_method,
        "conn_config": hand.conn_config,
    }
    result = get_service().create_hand(spec)
    return JSONResponse(content=result, status_code=201)


@hands_router.get("/vendor-info")
def get_vendor_info() -> dict[str, str]:
    return _VENDOR_INFO


@hands_router.get("/type")
def get_hand_type(
    query: str = Query(..., description="'product_type' or 'conn_type' or 'conn_config'"),
) -> dict[str, Any]:
    if query == "product_type":
        return {"type": get_supported_product_types()}
    if query == "conn_type":
        return {"type": get_supported_conn_types()}
    if query == "conn_config":
        return {"type": get_supported_conn_configs()}
    raise ValueError(f"Invalid query: {query}. Must be 'product_type', 'conn_type', or 'conn_config'.")


@hands_router.get("/methods")
def get_methods_by_product_type(
    product_type: str = Query(..., description="Product type key, e.g. 'omnihand_2025'"),
) -> Any:
    return get_service().describe_methods_by_product_type(product_type)


@hands_router.get("/{hand_id}/methods")
def get_hand_methods_by_id(hand_id: int) -> Any:
    return get_service().describe_methods(hand_id)


@hands_router.post("/{hand_id}/methods")
def call_hand_method_by_id(
    hand_id: int,
    method: str = Query(..., description="Method name to call"),
    body: dict[str, Any] = Body(default_factory=dict, description="Method parameters"),
) -> Any:
    return get_service().call_method(hand_id, method, body)


@hands_router.get("/{hand_id}")
def get_hand(hand_id: int) -> Any:
    return get_service().describe_hand(hand_id)


@hands_router.delete("/{hand_id}")
def release_hand(hand_id: int) -> Any:
    return get_service().remove_hand(hand_id)
