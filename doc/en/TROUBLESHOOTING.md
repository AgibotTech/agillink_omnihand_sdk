# Troubleshooting Guide

[中文](../zh_cn/TROUBLESHOOTING.md)

## Common Issues

### 1. Device Not Found / Connection Failed

**Symptoms:**
- `Failed to initialize`
- `CANFD device not found`
- `USB device not found`

**Solutions:**

#### Check Hardware Connection
```bash
# Linux: Check if device is recognized
lsusb | grep -i zlg
# Expected output: Bus XXX Device YYY: ID 3068:0009 ZLG USBCANFD-200U

# Check serial ports
ls -la /dev/ttyACM* /dev/ttyUSB*
```

#### Configure USB Permissions (Linux)
```bash
# Run setup script
sudo ./setup_udev.sh

# Log out and log back in (required!)

# Verify permissions
ls -la /dev/ttyACM0  # Should show rw-rw-rw-
```

#### Check Device Index
If you have multiple CANFD adapters:
```python
# Try different device indices
hand = OmniHand2025.create_hand_by_zlgcan(EHandType.LEFT, 1, 0, 0)  # First adapter
hand = OmniHand2025.create_hand_by_zlgcan(EHandType.LEFT, 1, 1, 0)  # Second adapter
```

---

### 2. Permission Denied

**Symptoms:**
- `Permission denied: /dev/ttyACM0`
- `LIBUSB_ERROR_ACCESS`
- Need to run with `sudo`

**Solutions:**

#### Linux
```bash
# Run udev setup
sudo ./setup_udev.sh

# Manually add user to groups
sudo usermod -aG dialout $USER
sudo usermod -aG plugdev $USER

# Log out and log back in

# Verify group membership
groups  # Should include 'dialout' and 'plugdev'
```

#### Windows
- Right-click and "Run as Administrator"
- Install ZLG USB driver if not installed

---

### 3. Communication Timeout

**Symptoms:**
- `Request timeout`
- `No response from device`
- Intermittent failures

**Solutions:**

#### Check CANFD Parameters
```python
# Verify hand_device_id matches the actual device
info = hand.get_device_info()
print(f"Actual device ID: {info.hand_device_id}")

# If mismatch, create with correct ID
hand = OmniHand2025.create_hand_by_zlgcan(EHandType.LEFT, actual_id, 0, 0)

# If you don't know canfd_device_id, use SN (serial number) interface:
hand = OmniHand2025.create_hand_by_zlgcan_sn(EHandType.LEFT, 1, "YOUR_DEVICE_SN", 0)
```

> **Note on `canfd_channel_id`**:
> - For dual-channel adapters (USBCANFD-200U): `can0` → `channel_id=0`, `can1` → `channel_id=1`
> - For single-channel adapters (USBCANFD-100U): always use `channel_id=0`

#### Check Power Supply
- Ensure 24V power is stable
- Check for loose power connections
- LED on hand should be lit

#### Reduce Communication Rate
```python
# Increase request interval
hand.set_request_interval(100)  # 100ms between requests

# Increase response timeout threshold
hand.set_request_timeout(500)  # 500ms timeout for each request
```

---

### 4. Motor Not Moving

**Symptoms:**
- Commands sent but no movement
- Position readings don't change

**Solutions:**

#### Check Control Mode
```python
# Use servo mode (recommended) or position mode
from omnihand import EControlMode

# Servo mode - smooth motion with velocity/torque limits
hand.set_all_control_mode(EControlMode.SERVO)

# Or position mode - direct position control
hand.set_all_control_mode(EControlMode.POSITION)
```

#### Check Position Range
```python
# Valid range is typically 0-4096
# WARNING: Some motors cannot reach extreme values (0 or 4096) due to finger mechanical limits
# Using extreme values may cause the motor to stall or trigger protection

# Start with safe middle positions (tested values from demo)
safe_pos = [2048, 2048, 4096, 2048, 4096, 4096, 2048, 4096, 2048, 4096]
hand.set_all_joint_motor_posi(safe_pos)

# Or use joint angle interface (recommended, handles limits automatically)
angles = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
hand.set_all_active_joint_angles(angles)
```

#### Check Error Status
```python
# Read error reports
errors = hand.get_all_error_report()
for i, err in enumerate(errors):
    if err:
        print(f"Joint {i+1} error: {err}")
```

---

### 5. Import Error (Python)

**Symptoms:**
- `ModuleNotFoundError: No module named 'omnihand'`
- `ImportError: DLL load failed`

**Solutions:**

#### Reinstall Package
```bash
# Uninstall and reinstall
pip uninstall omnihand
pip install python/omnihand-*.whl
```

#### Check Python Version
```bash
python3 --version  # Should be 3.10+
```

#### Install Test Dependencies (for running tests)
```bash
pip install pytest numpy
```

#### Windows DLL Issues
- Install Visual C++ Redistributable 2019+
- Ensure `zlgcan.dll` is accessible

---

### 6. CMake Find Package Failed (C++)

**Symptoms:**
- `Could not find package omnihand`
- `omnihand_DIR not set`

**Solutions:**

```cmake
# Set the correct path
set(OMNIHAND_ROOT "/usr/local")  # Linux default
# set(OMNIHAND_ROOT "C:/Program Files/omnihand2025")  # Windows

list(APPEND CMAKE_MODULE_PATH "${OMNIHAND_ROOT}/share/cmake/omnihand")
find_package(omnihand REQUIRED)
```

---

### 7. ROS2 Node Not Starting (Linux)

**Symptoms:**
- `Package 'omnihand_node' not found`
- Node fails to communicate

**Solutions:**

```bash
# Source ROS2 first
source /opt/ros/humble/setup.bash

# Then source omnihand
source ros2/setup.bash

# Verify package
ros2 pkg list | grep omnihand

# Check node
ros2 run omnihand_node omnihand_2025_node
```

---

## Diagnostic Commands

### Linux

```bash
# Check USB devices
lsusb -v | grep -A 20 "ZLG\|3068"

# Check serial ports
dmesg | tail -20

# Check udev rules
cat /etc/udev/rules.d/99-omnihand-usb.rules

# Check user groups
groups $USER

# Test device access
python3 -c "from omnihand import OmniHand2025; print('OK')"
```

### Windows

```cmd
# Check USB in Device Manager
devmgmt.msc

# Check Python
python --version
pip show omnihand
```

---

## Getting Help

If the issue persists:

1. **Collect information:**
   - OS version
   - SDK version (check `VERSION` file)
   - Error messages (full output)
   - Hardware model (O10/O12, adapter type)

2. **Check logs:**
   ```python
   # Enable verbose output
   import logging
   logging.basicConfig(level=logging.DEBUG)
   ```

3. **Contact support:**
   - Email: xuqigui@agibot.com
   - Include the collected information
