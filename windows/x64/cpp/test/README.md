# OmniHand 2025 SDK C++ Unit Tests

**API note:** For **OmniHand 2025 (O10)** and **OmniHand Dex UMI**, factory and broadcast APIs in the C++ headers use `std::uint8_t` for `hand_device_id` and `canfd_*` indices; tests use integer literals with implicit conversion. See `release/doc/en/API_CPP_O10.md` and `API_CPP_O10_UMI.md`.

This directory contains GoogleTest-based unit tests for all three OmniHand products:
- **OmniHand 2025 (O10)** - 10 DOF with 1D tactile sensors
- **OmniHand Pro 2025 (O12)** - 12 DOF with 3D tactile sensors
- **OmniHand Dex UMI (O10 UMI)** - 10 DOF with UMI protocol

## Building Tests

Tests are disabled by default. To enable tests, configure CMake with:

```bash
cmake -DBUILD_CPP_TESTS=ON ..
cmake --build .
```

Or if building from the project root:

```bash
cmake -DBUILD_CPP_TESTS=ON -B build
cmake --build build
```

## Running Tests

After building, run tests using CTest:

```bash
cd build
ctest
```

Or run individual test executables:

```bash
# Run O10 tests (default frequency: 10 Hz)
./cpp/test/test_omnihand_2025

# Run O10 tests with custom frequency (5-500 Hz)
./cpp/test/test_omnihand_2025 -f 20

# Run O12 tests (default frequency: 10 Hz)
./cpp/test/test_omnihand_pro_2025

# Run O12 tests with custom frequency (5-500 Hz)
./cpp/test/test_omnihand_pro_2025 -f 33

# Run O10 UMI tests
./cpp/test/test_omnihand_dex_umi
```

### Request Frequency Parameter

O10 and O12 test programs support a `-f FREQ` parameter to set the CAN request frequency:

- **Range**: 5-500 Hz
- **Default**: 10 Hz
- **Usage**: `./test_omnihand_2025 -f 20` (sets frequency to 20 Hz)

**Note**: UMI test program does not support frequency parameter (UMI protocol uses fixed periodic reports).

## Test Structure

Each test file (`test_omnihand_*.cc`) contains:

- **Factory Method Tests**: Verify object creation
- **Initialization Tests**: Test device initialization (requires hardware)
- **Device Info Tests**: Test device ID and vendor info
- **Motor Control Tests**: Test position, velocity, and angle control (requires hardware)
- **Sensor Tests**: Test tactile sensors and error reports (requires hardware)
- **Kinematics Tests**: Test forward kinematics calculations

## Hardware Requirements

Most tests require actual hardware to be connected:
- ZLG USB CANFD adapter
- OmniHand device connected and powered on

Tests that don't require hardware:
- Factory method creation
- Device info (without Init)
- Device ID setting

## Note

Tests are designed to gracefully handle missing hardware:
- Tests check `Init()` result before performing hardware-dependent operations
- Tests won't fail if hardware is not available (they will skip hardware-dependent assertions)

For CI/CD environments without hardware, you may want to:
1. Mock the hardware interfaces
2. Skip hardware-dependent tests
3. Use test fixtures that simulate hardware responses

## More Information

- [中文文档](README_zh_cn.md)
