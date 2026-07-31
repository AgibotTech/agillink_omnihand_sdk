"""Lazy loader and capability helpers for the omnihand SDK.

All SDK symbols are resolved on first use so the FastAPI application can
be imported (and tested) even when the native extension is not installed.
"""

from typing import Any
import logging
from pathlib import Path

import toml

logger = logging.getLogger(__name__)

_omnihand: Any = None
_hand_classes: dict[str, type] = {}
_product_types: dict[str, int] = {}

_PRODUCT_TYPE_DEFS = [
    ("omnihand_2025", "OmniHand2025", "OMNIHAND_2025"),
    ("omnihand_pro_2025", "OmniHandPro2025", "OMNIHAND_PRO_2025"),
    ("omnihand_dex_umi", "OmniHandDexUMI", "OMNIHAND_DEX_UMI"),
    ("omnihand_3_lite", "OmniHand3Lite", "OMNIHAND_3_LITE"),
    ("omnihand_3_ultra_m", "OmniHand3UltraM", "OMNIHAND_3_ULTRA_M"),
]

_CONN_METHOD_FACTORY_CANDIDATES: dict[str, tuple[str, ...]] = {
    "zlgcan": ("create_hand_by_zlgcan",),
    "hcan": ("create_hand_by_hcan",),
    "socketcan": ("create_hand_socketcan",),
    "rs485": ("create_hand_by_rs485",),
    "usb": ("create_hand_by_usb", "create_hand_by_rs485"),
    "tj": ("create_hand_by_tj",),
    "zlgcan_tcp": ("create_hand_by_zlgcan_tcp",),
}

_CONN_PARAM_ALIASES: dict[str, dict[str, tuple[str, ...]]] = {
    "socketcan": {
        "can_interface": ("can_interface", "can_if"),
    },
    "zlgcan_tcp": {
        "host": ("host", "tcp_host"),
        "port": ("port", "tcp_port"),
    },
}


def _load_hand_create_config() -> dict[str, Any]:
    try:
        return toml.loads(
            (Path(__file__).resolve().parent.parent / "configs" / "hand_create.toml").read_text()
        )
    except (FileNotFoundError, toml.TomlDecodeError) as exc:
        logger.warning("Failed to load hand_create.toml: %s. Using defaults.", exc)
        return {}


def _ensure_sdk() -> None:
    global _omnihand, _hand_classes, _product_types
    if _omnihand is not None:
        return

    import omnihand as mod

    _omnihand = mod
    for hand_key, cls_name, pt_name in _PRODUCT_TYPE_DEFS:
        cls = getattr(mod, cls_name, None)
        pt = getattr(mod.ProductType, pt_name, None)
        if cls is None or pt is None:
            logger.warning(
                "SDK does not support %s (missing %s or ProductType.%s), skipping.",
                hand_key,
                cls_name,
                pt_name,
            )
            continue
        _hand_classes[hand_key] = cls
        _product_types[hand_key] = pt


def get_sdk_module() -> Any:
    _ensure_sdk()
    return _omnihand


def get_hand_class(hand_type: str) -> type:
    _ensure_sdk()
    cls = _hand_classes.get(hand_type)
    if cls is None:
        raise ValueError(f"Unknown hand type: {hand_type}")
    return cls


def get_product_type(hand_type: str) -> int:
    _ensure_sdk()
    pt = _product_types.get(hand_type)
    if pt is None:
        raise ValueError(f"Unknown hand type: {hand_type}")
    return pt


def get_supported_product_types() -> list[str]:
    configured = _load_hand_create_config().get("omnihand", {}).get("product_type", [])
    if configured:
        configured = [name for name in configured if name in {item[0] for item in _PRODUCT_TYPE_DEFS}]
    _ensure_sdk()
    available = [name for name, *_ in _PRODUCT_TYPE_DEFS if name in _hand_classes]
    if configured:
        return [name for name in configured if name in available]
    return available


def get_supported_conn_configs() -> dict[str, list[dict[str, str]]]:
    configs = _load_hand_create_config().get("omnihand", {}).get("conn_config", {})
    return {name: list(variants) for name, variants in configs.items()}


def get_supported_conn_types() -> list[str]:
    configured = _load_hand_create_config().get("omnihand", {}).get("conn_type", [])
    if configured:
        return [name for name in configured if name in _CONN_METHOD_FACTORY_CANDIDATES]
    return list(_CONN_METHOD_FACTORY_CANDIDATES)


def get_factory_name(hand_type: str, conn_method: str) -> str:
    hand_cls = get_hand_class(hand_type)
    candidates = _CONN_METHOD_FACTORY_CANDIDATES.get(conn_method)
    if candidates is None:
        raise ValueError(f"Unknown connection method: {conn_method}")

    for name in candidates:
        if hasattr(hand_cls, name):
            return name
    raise ValueError(f"Connection method '{conn_method}' not available for {hand_type}")


def get_param_aliases(conn_method: str) -> dict[str, tuple[str, ...]]:
    return _CONN_PARAM_ALIASES.get(conn_method, {})
