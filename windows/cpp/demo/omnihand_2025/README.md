# OmniHand 2025 SDK C++ Demos

This directory contains C++ demo programs for the OmniHand 2025 SDK, demonstrating different communication methods.

## Demo List

All demos support three control modes:
- `left` - Control left hand only
- `right` - Control right hand only
- `both` - Control both hands simultaneously

### 1. `O10_demo_canfd_id.cc`
**CANFD Communication (by canfd_id)**

Create and control the hand using USB CANFD adapter device index.

```bash
# Control left hand
./demo_omnihand_2025_canfd_id left

# Control right hand
./demo_omnihand_2025_canfd_id right

# Control both hands (using different channels of the same adapter)
./demo_omnihand_2025_canfd_id both
```

**Use Cases**:
- Single USB CANFD adapter
- Know the adapter index (usually 0)
- `both` mode requires multi-channel adapter (e.g., USBCANFD-200U has 2 channels)

### 2. `O10_demo_canfd_serial.cc`
**CANFD Communication (by serial_number)**

Create and control the hand using USB CANFD adapter serial number.

```bash
./demo_omnihand_2025_canfd_serial left
./demo_omnihand_2025_canfd_serial right
./demo_omnihand_2025_canfd_serial both
```

**Use Cases**:
- Multiple USB CANFD adapters
- Need to precisely specify device by serial number
- Serial number supports partial matching (e.g., "201BFF2A" can match "201BFF2AF01202D44690USBCANFD-200U")

**Note**: Serial number in code needs to be modified according to actual device.

### 3. `O10_demo_rs485.cc`
**RS485 Communication**

Create and control the hand using RS485 serial communication.

```bash
./demo_omnihand_2025_rs485 left
./demo_omnihand_2025_rs485 right
./demo_omnihand_2025_rs485 both
```

**Use Cases**:
- Using RS485 serial communication
- Need to modify serial port path in code (e.g., `/dev/ttyUSB0` or `COM3`)

**Note**:
- Linux: Ensure serial port access permission `sudo chmod 666 /dev/ttyUSB0`
- Windows: Use COM port name, e.g., `COM3`
- `both` mode requires two different serial ports

### 4. `O10_demo_socketcan.cc` (Linux only)
**SocketCAN Communication**

Create and control the hand using Linux SocketCAN interface.

```bash
# First configure CAN interface
sudo ip link set can0 type can bitrate 1000000 sample-point 0.8 dbitrate 5000000 dsample-point 0.8 fd on
sudo ip link set can0 up
sudo ip link set can1 type can bitrate 1000000 sample-point 0.8 dbitrate 5000000 dsample-point 0.8 fd on
sudo ip link set can1 up

# Run demo
./demo_omnihand_2025_socketcan left    # Use can0
./demo_omnihand_2025_socketcan right   # Use can0
./demo_omnihand_2025_socketcan both    # Use can0 and can1
```

**Use Cases**:
- Existing SocketCAN environment (onboard CAN, other SocketCAN devices)
- For USB CANFD devices, ZLG library method is recommended (no driver configuration needed)

## Building

### Method 1: Build as part of SDK

```bash
cd /path/to/Omnihand-2025-SDK
mkdir build && cd build
cmake .. -DBUILD_CPP_DEMO=ON
make
```

### Method 2: Standalone build (requires SDK installation)

```bash
cd cpp/demo/omnihand_2025
mkdir build && cd build
cmake ..
make
```

## Running

Compiled executables are located in `build/bin/omnihand/demo/` directory:

```bash
# CANFD (by canfd_id)
./build/bin/omnihand/demo/demo_omnihand_2025_canfd_id left
./build/bin/omnihand/demo/demo_omnihand_2025_canfd_id right
./build/bin/omnihand/demo/demo_omnihand_2025_canfd_id both

# CANFD (by serial_number)
./build/bin/omnihand/demo/demo_omnihand_2025_canfd_serial left
./build/bin/omnihand/demo/demo_omnihand_2025_canfd_serial right
./build/bin/omnihand/demo/demo_omnihand_2025_canfd_serial both

# RS485
./build/bin/omnihand/demo/demo_omnihand_2025_rs485 left
./build/bin/omnihand/demo/demo_omnihand_2025_rs485 right
./build/bin/omnihand/demo/demo_omnihand_2025_rs485 both

# SocketCAN (Linux only)
./build/bin/omnihand/demo/demo_omnihand_2025_socketcan left
./build/bin/omnihand/demo/demo_omnihand_2025_socketcan right
./build/bin/omnihand/demo/demo_omnihand_2025_socketcan both
```

## Communication Method Comparison

| Communication Method | Advantages | Disadvantages | Recommended Scenarios |
|---------------------|------------|---------------|---------------------|
| **CANFD (canfd_id)** | Simple, zero configuration | Need to know adapter index | Single adapter, simple scenarios |
| **CANFD (serial_number)** | Precise device specification, supports multiple adapters | Need to know serial number | Multiple adapters, need precise control |
| **RS485** | Serial communication, good compatibility | Slower speed, requires serial port permission | Serial port environment, compatibility requirements |
| **SocketCAN** | Native Linux support | Requires driver and interface configuration | Linux environment, existing SocketCAN |

## Notes

1. **Device ID**: Ensure `device_id` in code matches the device ID configured in hardware
2. **Channel Selection**:
   - `left`/`right` mode: Use first channel (channel_id=0)
   - `both` mode: Left hand uses channel_id=0, right hand uses channel_id=1 (requires multi-channel adapter)
3. **Serial Number Matching**: When using serial number, partial matching is supported, but uniqueness must be ensured
4. **Permission Issues**:
   - RS485: Linux requires serial port access permission
   - SocketCAN: Requires root permission to configure interface
5. **Frequency Control**: SDK internally implements request frequency control (default 10 Hz), no need to manually add delays

## More Information

- [API Documentation](../../../doc/en/API_CPP_O10.md)
- [中文文档](README_zh_cn.md)
