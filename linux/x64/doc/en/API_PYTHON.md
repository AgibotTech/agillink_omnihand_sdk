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
- **[OmniHand Dex UMI (O10 UMI) Python API](API_PYTHON_UMI.md)** - 10 DOF, UMI protocol, periodic reports, CAN only

## Common Enumerations

All products share common enumerations. These are documented in each product-specific API document, but here's a quick reference:

### EHandType

```python
from omnihand import EHandType

# Values
EHandType.LEFT = 0      # Left hand
EHandType.RIGHT = 1     # Right hand
EHandType.UNKNOWN = 10
```

### EFinger

```python
from omnihand import EFinger

# Values
EFinger.THUMB = 1
EFinger.INDEX = 2
EFinger.MIDDLE = 3
EFinger.RING = 4
EFinger.LITTLE = 5
EFinger.PALM = 6
EFinger.DORSUM = 7
EFinger.UNKNOWN = 255
```

### EControlMode

```python
from omnihand import EControlMode

# Values
EControlMode.POSITION = 0
EControlMode.SERVO = 1            # Servo mode
EControlMode.VELOCITY = 2
EControlMode.TORQUE = 3           # Not supported (use mixed modes instead)
EControlMode.POSITION_TORQUE = 4  # Mixed control
EControlMode.VELOCITY_TORQUE = 5  # Mixed control
EControlMode.POSITION_VELOCITY_TORQUE = 6  # Mixed control
EControlMode.UNKNOWN = 10
```

**Note**: 
- **SERVO mode**: Servo control mode
- **Pure torque control (TORQUE)**: Not supported by either O10 or O12. Use mixed control modes instead

## Quick Start Examples

### OmniHand 2025 (O10)

```python
from omnihand import OmniHand2025, EHandType

# Create hand instance
hand = OmniHand2025.create_hand_by_zlgcan(
    hand_type=EHandType.LEFT,
    device_id=1,
    canfd_id=0,
    channel_id=0
)

if not hand.init():
    print("Failed to initialize")
    exit(1)

# Set motor positions (range: 0-4096 for O10)
positions = [500, 2081, 4094, 2029, 4094, 4094, 2048, 4094, 4000, 4094]
hand.set_all_joint_positions(positions)
```

### OmniHand Pro 2025 (O12)

```python
from omnihand import OmniHandPro2025, EHandType

# Create hand instance
hand = OmniHandPro2025.create_hand_by_zlgcan(
    hand_type=EHandType.LEFT,
    device_id=1,
    canfd_id=0,
    channel_id=0
)

if not hand.init():
    print("Failed to initialize")
    exit(1)

# Set motor positions (range: 0-2000 for O12)
positions = [500, 1000, 1500, 2000, 1000, 1500, 500, 1000, 1500, 2000, 1000, 1500]
hand.set_all_joint_positions(positions)
```

### OmniHand Dex UMI

```python
from omnihand import OmniHandDexUMI, EHandType

# Create hand instance
hand = OmniHandDexUMI.create_hand_by_zlgcan(
    hand_type=EHandType.LEFT,
    device_id=1,
    canfd_id=0,
    channel_id=0
)

if not hand.init():
    print("Failed to initialize")
    exit(1)

# Register position report callback
def position_callback(positions):
    print(f"Position report: {len(positions)} values")

hand.set_position_report_callback(position_callback, frequency=100)  # 100 Hz
```

## Migration from Unified Interface

If you were using the old unified `OmniHand.create_hand_by_zlgcan(ProductType, ...)` interface:

**Old code:**
```python
from omnihand import OmniHand, ProductType, EHandType

hand = OmniHand.create_hand_by_zlgcan(
    ProductType.OMNIHAND_2025,
    hand_type=EHandType.LEFT,
    device_id=1,
    canfd_id=0,
    channel_id=0
)
```

**New code:**
```python
from omnihand import OmniHand2025, EHandType

hand = OmniHand2025.create_hand_by_zlgcan(
    hand_type=EHandType.LEFT,
    device_id=1,
    canfd_id=0,
    channel_id=0
)
```

The product type is now determined by the class you use, providing better type safety and compile-time checks.

## Related Documentation

- [SocketCAN Setup Guide](SOCKETCAN_SETUP.md) - For Linux SocketCAN configuration
- [OmniHand 2025 (O10) Kinematics Solver Python API](API_KINEMATICS_PYTHON_O10.md) - For kinematics calculations
- [OmniHand Pro 2025 (O12) Kinematics Solver Python API](API_KINEMATICS_PYTHON_O12.md) - For kinematics calculations
