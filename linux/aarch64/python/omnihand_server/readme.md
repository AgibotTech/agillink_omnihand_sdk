# Omnihand SDK API

## Dependencies

- FastAPI
- Uvicorn
- Python 3.10+

## Development Environment
1. Install dependencies:
   ```bash
   pip install fastapi uvicorn toml
   ```

2. Run the server (from repo root):
   ```bash
   PYTHONPATH=python uvicorn omnihand_server.app.main:app --reload --port 8000
   ```

   Or install the package in editable mode first:
   ```bash
   pip install -e python/
   uvicorn omnihand_server.app.main:app --reload --port 8000
   ```

3. Test:
   ```bash
   PYTEST_DISABLE_PLUGIN_AUTOLOAD=1 python -m pytest python/test/server/ -v 2>&1 | tail -100
   ```
