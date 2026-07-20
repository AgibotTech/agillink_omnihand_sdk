# Copyright (c) 2025, Agibot Co., Ltd.
"""Singleton wrapper around the C++ pybind11 HandService binding.

All method routing, session management, and stream reading is delegated to
the C++ HandService.  Import this module lazily so the app can be imported
even when the native extension is not installed (tests skip via AVAILABLE).
"""

_service = None


def get_service():
    """Return (and lazily create) the C++ HandService singleton."""
    global _service
    if _service is None:
        from omnihand.omnihand_core import HandService  # noqa: PLC0415
        _service = HandService()
    return _service


def shutdown() -> None:
    """Destroy the C++ HandService (closes all open hand sessions)."""
    global _service
    if _service is not None:
        del _service
        _service = None
