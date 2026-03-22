# OmniHand 2025 SDK Python API

## Overview

The OmniHand 2025 SDK provides **product-specific interfaces** for three different products:

- **OmniHand 2025 (O10)**: 10 DOF dexterous hand with 1D tactile sensors
- **OmniHand Pro 2025 (O12)**: 12 DOF dexterous hand with 3D tactile sensors
- **OmniHand Dex UMI (O10 UMI)**: 10 DOF dexterous hand with UMI protocol support

Each product has its own interface class (`OmniHand2025`, `OmniHandPro2025`, `OmniHandDexUMI`) with product-specific factory methods and APIs. This design provides better type safety and clearer API organization compared to a unified interface with `ProductType`.

## Product-Specific API Documentation

- **[OmniHand 2025 (O10) Python API](API_PYTHON_O10.md)** - 10 DOF, 1D tactile sensors, supports CAN and RS485
- **[OmniHand Pro 2025 (O12) Python API](API_PYTHON_O12.md)** - 12 DOF, 3D tactile sensors, CAN only
- **[OmniHand Dex UMI (O10 UMI) Python API](API_PYTHON_UMI.md)** - 10 DOF, UMI protocol, active query, CAN only

## Common Enumerations

All products share common enumerations. These are documented in each product-specific API document, but here's a quick reference:

### HandType

```python
from omnihand import HandType

# Values
HandType.LEFT = 0              # Left hand
HandType.RIGHT = 1             # Right hand
HandType.UNKNOWN = 255  # Unknown hand type
```

### Finger

```python
from omnihand import Finger

# Values
Finger.THUMB = 1
Finger.INDEX = 2
Finger.MIDDLE = 3
Finger.RING = 4
Finger.LITTLE = 5
Finger.PALM = 6
Finger.DORSUM = 7
Finger.UNKNOWN = 255
```

### ControlMode

```python
from omnihand import ControlMode

# Values
ControlMode.POSITION = 0
ControlMode.SERVO = 1            # Servo mode
ControlMode.VELOCITY = 2
ControlMode.TORQUE = 3           # Not supported (use mixed modes instead)
ControlMode.POSITION_TORQUE = 4  # Mixed control
ControlMode.VELOCITY_TORQUE = 5  # Mixed control
ControlMode.POSITION_VELOCITY_TORQUE = 6  # Mixed control
ControlMode.UNKNOWN = 10
```

**Note**: 
- **SERVO mode**: Servo control mode
- **Pure torque control (TORQUE)**: Not supported by either O10 or O12. Use mixed control modes instead

## Quick Start Examples

### OmniHand 2025 (O10)

```python
from omnihand import OmniHand2025, HandType

# Create hand instance
hand = OmniHand2025.create_hand_by_zlgcan(
    hand_type=HandType.LEFT,
    hand_device_id=1,
    canfd_device_id=0,
    canfd_channel_id=0,
)

if not hand.init():
    print("Failed to initialize")
    exit(1)

# Set joint angles (unit: radians, 10 joints for O10)
angles = [0.0] * 10  # All joints to zero position
hand.set_all_active_joint_angles(angles)
```

### OmniHand Pro 2025 (O12)

```python
from omnihand import OmniHandPro2025, HandType

# Create hand instance
hand = OmniHandPro2025.create_hand_by_zlgcan(
    hand_type=HandType.LEFT,
    hand_device_id=1,
    canfd_device_id=0,
    canfd_channel_id=0,
)

if not hand.init():
    print("Failed to initialize")
    exit(1)

# Set joint angles (unit: radians, 12 joints for O12)
angles = [0.0] * 12  # All joints to zero position
hand.set_all_active_joint_angles(angles)
```

### OmniHand Dex UMI

```python
from omnihand import OmniHandDexUMI, HandType

# Create hand instance
hand = OmniHandDexUMI.create_hand_by_zlgcan(
    hand_type=HandType.LEFT,
    hand_device_id=1,
    canfd_device_id=0,
    canfd_channel_id=0,
)

if not hand.init():
    print("Failed to initialize")
    exit(1)

# Register position report callback
def position_callback(positions):
    print(f"Position report: {len(positions)} values")

hand.set_position_report_callback(position_callback, frequency=100)  # 100 Hz
```

## Related Documentation

- [SocketCAN Setup Guide](SOCKETCAN_SETUP.md) - For Linux SocketCAN configuration
- [OmniHand 2025 (O10) Kinematics Solver Python API](API_KINEMATICS_PYTHON_O10.md) - For kinematics calculations
- [OmniHand Pro 2025 (O12) Kinematics Solver Python API](API_KINEMATICS_PYTHON_O12.md) - For kinematics calculations
