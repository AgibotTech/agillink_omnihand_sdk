# OmniHand Dex UMI SDK C++ Demos

This directory contains C++ demo programs for the OmniHand Dex UMI SDK, demonstrating different communication methods.

## Product Features

- **10 Degrees of Freedom**: Same DOF as OmniHand 2025
- **UMI Protocol**: Uses UMI protocol (Pn1-Pn7 registers)
- **Read-Only Mode**: **Position/velocity/torque control not supported**, only supports reading position and tactile sensor data
- **Periodic Reports**: Supports periodic position and tactile sensor data reporting
- **CAN Communication Only**: RS485 not supported

## Demo List

All demos support three modes:
- `left` - Read left hand data only
- `right` - Read right hand data only
- `both` - Read both hands data simultaneously

⚠️ **Important**: UMI protocol is read-only. All demos are for reading data only, control operations are not supported.

### 1. `UMI_demo_canfd_id.cc`
**CANFD Communication (by canfd_id)**

Create and read hand data using USB CANFD adapter device index.

```bash
# Read left hand data
./demo_omnihand_dex_umi_canfd_id left

# Read right hand data
./demo_omnihand_dex_umi_canfd_id right

# Read both hands data (using different channels of the same adapter)
./demo_omnihand_dex_umi_canfd_id both
```

**Use Cases**:
- Single USB CANFD adapter
- Know the adapter index (usually 0)
- `both` mode requires multi-channel adapter (e.g., USBCANFD-200U has 2 channels)

### 2. `UMI_demo_canfd_serial.cc`
**CANFD Communication (by serial_number)**

Create and read hand data using USB CANFD adapter serial number.

```bash
./demo_omnihand_dex_umi_canfd_serial left
./demo_omnihand_dex_umi_canfd_serial right
./demo_omnihand_dex_umi_canfd_serial both
```

**Use Cases**:
- Multiple USB CANFD adapters
- Need to precisely specify device by serial number
- Serial number supports partial matching (e.g., "201BFF2A" can match "201BFF2AF01202D44690USBCANFD-200U")

**Note**: Serial number in code needs to be modified according to actual device.

### 3. `UMI_demo_socketcan.cc` (Linux only)
**SocketCAN Communication**

Create and read hand data using Linux SocketCAN interface.

```bash
# First configure CAN interface
sudo ip link set can0 type can bitrate 1000000 sample-point 0.8 dbitrate 5000000 dsample-point 0.8 fd on
sudo ip link set can0 up
sudo ip link set can1 type can bitrate 1000000 sample-point 0.8 dbitrate 5000000 dsample-point 0.8 fd on
sudo ip link set can1 up

# Run demo
./demo_omnihand_dex_umi_socketcan left    # Use can0
./demo_omnihand_dex_umi_socketcan right   # Use can0
./demo_omnihand_dex_umi_socketcan both    # Use can0 and can1
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
cd cpp/demo/omnihand_dex_umi
mkdir build && cd build
cmake ..
make
```

## Running

Compiled executables are located in `build/bin/omnihand/demo/` directory:

```bash
# CANFD (by canfd_id)
./build/bin/omnihand/demo/demo_omnihand_dex_umi_canfd_id left
./build/bin/omnihand/demo/demo_omnihand_dex_umi_canfd_id right
./build/bin/omnihand/demo/demo_omnihand_dex_umi_canfd_id both

# CANFD (by serial_number)
./build/bin/omnihand/demo/demo_omnihand_dex_umi_canfd_serial left
./build/bin/omnihand/demo/demo_omnihand_dex_umi_canfd_serial right
./build/bin/omnihand/demo/demo_omnihand_dex_umi_canfd_serial both

# SocketCAN (Linux only)
./build/bin/omnihand/demo/demo_omnihand_dex_umi_socketcan left
./build/bin/omnihand/demo/demo_omnihand_dex_umi_socketcan right
./build/bin/omnihand/demo/demo_omnihand_dex_umi_socketcan both
```

## Communication Method Comparison

| Communication Method | Advantages | Disadvantages | Recommended Scenarios |
|---------------------|------------|---------------|---------------------|
| **CANFD (canfd_id)** | Simple, zero configuration | Need to know adapter index | Single adapter, simple scenarios |
| **CANFD (serial_number)** | Precise device specification, supports multiple adapters | Need to know serial number | Multiple adapters, need precise control |
| **SocketCAN** | Native Linux support | Requires driver and interface configuration | Linux environment, existing SocketCAN |

## Notes

1. **Read-Only Mode**: ⚠️ **UMI protocol does not support position/velocity/torque control**, all demos are for reading data only
2. **Device ID**: Ensure `device_id` in code matches the device ID configured in hardware
3. **Channel Selection**:
   - `left`/`right` mode: Use first channel (channel_id=0)
   - `both` mode: Left hand uses channel_id=0, right hand uses channel_id=1 (requires multi-channel adapter)
4. **Serial Number Matching**: When using serial number, partial matching is supported, but uniqueness must be ensured
5. **Permission Issues**:
   - SocketCAN: Requires root permission to configure interface
6. **Periodic Reports**: UMI supports periodic position and tactile sensor data reporting, can be received via callback functions
7. **Tactile Sensors**: UMI uses 1D tactile sensors, use `GetTactileSensorData()` method to read
8. **UMI Protocol**: Uses Pn1-Pn7 registers, supports configuring report frequency and other parameters

## More Information

- [API Documentation](../../../doc/en/API_CPP_O10_UMI.md)
- [中文文档](README_zh_cn.md)
