# Python Unit Tests

This directory contains unit tests for the AGILINK OmniHand SDK Python interface, using pytest framework.

**API note:** O10/Dex UMI factory parameters such as `hand_device_id`, `canfd_device_id`, and `canfd_channel_id` are passed as Python `int` and forwarded to C++ as `uint8_t` (see `release/doc/en/API_PYTHON_O10.md`).

## Requirements

Install pytest:

```bash
pip install pytest
```

## Running Tests

### Run all tests

```bash
cd python/test
pytest -v
```

### Run tests for a specific product

```bash
# O10 tests (using pytest directly)
pytest -v test_omnihand_2025.py

# O10 tests (running directly with Python - supports -f parameter)
python3 test_omnihand_2025.py -f 0

# O12 tests (using pytest directly)
pytest -v test_omnihand_pro_2025_pytest.py

# O12 tests (running directly with Python - supports -f parameter)
python3 test_omnihand_pro_2025_pytest.py -f 0

# UMI tests (using pytest directly)
pytest -v test_omnihand_dex_umi.py

# UMI tests (running directly with Python)
python3 test_omnihand_dex_umi.py
```

**Note**: 
- When running directly with Python, the `-f` parameter specifies the request interval in milliseconds (0-100ms). Use `0` to disable interval limiting.
- When using `pytest` directly, use environment variable: `OMNIHAND_REQUEST_INTERVAL=0 pytest -v test_omnihand_2025.py`
- UMI tests do not support the `-f` interval parameter, as the UMI protocol uses fixed periodic reports.
- Running tests directly with Python automatically enables verbose mode (`-v`) to show detailed test results.

### Verbose output

```bash
pytest -v
```

### Show print statements

```bash
pytest -s
```

## Test Structure

The tests are organized similar to the C++ gtest structure:

- **test_omnihand_2025.py**: Tests for OmniHand 2025 (O10, 10 DOF)
- **test_omnihand_pro_2025.py**: Tests for OmniHand Pro 2025 (O12, 12 DOF)
- **test_omnihand_dex_umi.py**: Tests for OmniHand Dex UMI (UMI protocol)

## Test Coverage

Each test file covers:

- Factory methods (create_hand)
- Initialization
- Vendor and device information
- Device ID setting (with proper cleanup)
- Joint angle control
- Control mode (read-only)
- Tactile sensors (product-specific)
- Error reports
- Temperature reports
- Current reports
- Kinematics solver
- UMI-specific features (callbacks, report frequencies)

## Notes

- Tests require hardware to be connected
- Tests will skip if device initialization fails
- Tests handle timeouts gracefully (skip instead of fail)
- Similar to C++ tests, `set_all_control_modes` is not tested as it may cause CANFD communication issues

## More Information

- [中文文档](README_zh_cn.md)
