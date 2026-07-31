import asyncio
import logging
from contextlib import asynccontextmanager
from datetime import datetime
from pathlib import Path
from typing import AsyncIterator

from fastapi import FastAPI, Request
from fastapi.middleware.cors import CORSMiddleware
from fastapi.responses import JSONResponse

from omnihand_server.core.service import get_service, shutdown
from omnihand_server.routers.hand_router import hands_router
from omnihand_server.routers.ws import ws_router

_now = datetime.now()
_log_dir = Path(__file__).resolve().parent.parent.parent / ".log" / _now.strftime("%Y-%m-%d")
_log_dir.mkdir(parents=True, exist_ok=True)
_log_file = _log_dir / _now.strftime("%H-%M-%S.log")

_root_logger = logging.getLogger()
_root_logger.setLevel(logging.INFO)
_fmt = logging.Formatter("%(asctime)s %(levelname)s %(name)s: %(message)s")
_fh = logging.FileHandler(_log_file)
_fh.setFormatter(_fmt)
_root_logger.addHandler(_fh)
logger = logging.getLogger(__name__)


@asynccontextmanager
async def _lifespan(app: FastAPI) -> AsyncIterator[None]:
    try:
        yield
    finally:
        await asyncio.get_event_loop().run_in_executor(None, shutdown)


def create_app() -> FastAPI:
    app = FastAPI(title="OmniHand API", version="1.1.0", lifespan=_lifespan)

    app.add_middleware(
        CORSMiddleware,
        allow_origins=["*"],
        allow_credentials=True,
        allow_methods=["*"],
        allow_headers=["*"],
    )

    @app.exception_handler(IndexError)
    async def index_error_handler(request: Request, exc: IndexError) -> JSONResponse:
        return JSONResponse(status_code=404, content={"detail": str(exc)})

    @app.exception_handler(ValueError)
    async def value_error_handler(request: Request, exc: ValueError) -> JSONResponse:
        return JSONResponse(status_code=400, content={"detail": str(exc)})

    @app.exception_handler(Exception)
    async def generic_exception_handler(request: Request, exc: Exception) -> JSONResponse:
        logger.exception("Unhandled exception on %s %s", request.method, request.url)
        return JSONResponse(status_code=500, content={"detail": "Internal server error"})

    app.include_router(hands_router, prefix="/v1/hands")
    app.include_router(ws_router)

    @app.get("/health")
    def health() -> JSONResponse:
        hands = get_service().list_hands()
        return JSONResponse(status_code=200, content={"status": "ok", "hands": len(hands)})

    return app


app = create_app()
