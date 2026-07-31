"""Shared fixtures for omnihand_server tests."""

import pytest

try:
    from fastapi.testclient import TestClient
    from omnihand_server.app.main import app
    AVAILABLE = True
except ImportError:
    AVAILABLE = False


@pytest.fixture(scope="module")
def client():
    with TestClient(app) as c:
        yield c
