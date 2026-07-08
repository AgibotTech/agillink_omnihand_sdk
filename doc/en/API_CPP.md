# OmniHand SDK C++ API

## Overview

The OmniHand SDK provides **product-specific interfaces** for four different products:

- **OmniHand 2025 (O10)**: 10 DOF dexterous hand with 1D tactile sensors
- **OmniHand Pro 2025 (O12)**: 12 DOF dexterous hand with 3D tactile sensors
- **OmniHand 3 Lite (H3L)**: 4 DOF lightweight dexterous hand
- **OmniHand Dex UMI (O10 UMI)**: 10 DOF dexterous hand with UMI protocol support
- **OmniPicker 3 (OP3)**: 1 DOF gripper

Each product has its own interface class (`OmniHand2025`, `OmniHandPro2025`, `OmniHand3Lite`, `OmniHandDexUMI`) with product-specific factory methods and APIs. This design provides better type safety and clearer API organization compared to a unified interface with `ProductType`.

**C++ typing note:** For **OmniHand 2025 (O10)** and **OmniHand Dex UMI**, factory methods use **`std::uint8_t`** for `hand_device_id` and CAN adapter indices (`canfd_device_id`, `canfd_channel_id`). See the product-specific C++ API pages for full signatures.

## Product-Specific API Documentation

- **[OmniHand 2025 (O10) C++ API](API_CPP_O10.md)** - 10 DOF, 1D tactile sensors, supports CAN and RS485
- **[OmniHand Pro 2025 (O12) C++ API](API_CPP_O12.md)** - 12 DOF, 3D tactile sensors, CAN only
- **[OmniHand 3 Lite (H3L) C++ API](API_CPP_H3L.md)** - 4 DOF, no tactile sensors, CAN only
- **[OmniHand Dex UMI (O10 UMI) C++ API](API_CPP_O10_UMI.md)** - 10 DOF, UMI protocol, active query, CAN only
- **[OmniPicker 3 (OP3) C++ API](API_CPP_OP3.md)** - 1 DOF gripper; mixed control: `MixControlByPT` only

## Common Enums and Data Structures

All products share common enums and data structures. These are documented in each product-specific API document, but here's a quick reference:

### ProductType (for reference only, not used in new API)

```cpp
enum class ProductType : unsigned char {
    OMNIHAND_2025        = 0,    // OmniHand 2025 (10 DOF)
    OMNIHAND_PRO_2025    = 1,    // OmniHand Pro 2025 (12 DOF)
    OMNIHAND_DEX_UMI     = 2,    // OmniHand Dex UMI (O10 UMI) (10 DOF, UMI protocol)
    UNKNOWN = 255   // Unknown
};
```

**Note**: The new product-specific interfaces do not require `ProductType` - each class is already typed for its product.

### HandType

```cpp
enum class HandType : unsigned char {
    LEFT = 0,      // Left hand
    RIGHT = 1,     // Right hand
    UNKNOWN = 255  // Unknown hand type
};
```

### Finger

```cpp
enum class Finger : unsigned char {
    THUMB = 0x01,    // Thumb
    INDEX = 0x02,    // Index finger
    MIDDLE = 0x03,   // Middle finger
    RING = 0x04,     // Ring finger
    LITTLE = 0x05,   // Little (pinky) finger
    PALM = 0x06,     // Palm
    DORSUM = 0x07,   // Dorsum (back of hand)
    UNKNOWN = 0xff   // Unknown
};
```

### ControlMode

```cpp
enum class ControlMode : unsigned char {
    POSITION           = 0,    // Position mode
    SERVO          = 1,    // Servo mode
    VELOCITY           = 2,    // Velocity mode
    TORQUE         = 3,    // Torque mode (Not supported: pure torque control not available)
    POSITION_TORQUE     = 4,    // Position-Torque mode (Mixed control: position + torque)
    VELOCITY_TORQUE     = 5,    // Velocity-Torque mode (Mixed control: velocity + torque)
    POSITION_VELOCITY_TORQUE = 6,    // Position-Velocity-Torque mode (Mixed control: position + velocity + torque)
    UNKNOWN        = 10    // Unknown mode
};
```

## Quick Start Examples

### OmniHand 2025 (O10)

```cpp
#include "omnihand/omnihand_2025.h"

// Create hand instance
auto hand = OmniHand2025::createHandByZlgcan(
    HandType::LEFT,
    1,      // hand_device_id
    0,      // canfd_device_id
    0       // canfd_channel_id
);

if (!hand || !hand->Init()) {
    std::cerr << "Failed to initialize" << std::endl;
    return -1;
}

// Set joint angles (unit: radians, 10 joints for O10)
std::vector<double> angles(10, 0.0);  // All joints to zero position
hand->SetAllActiveJointAngles(angles);
```

### OmniHand Pro 2025 (O12)

```cpp
#include "omnihand/omnihand_pro_2025.h"

// Create hand instance
auto hand = OmniHandPro2025::createHandByZlgcan(
    HandType::LEFT,
    1,      // hand_device_id
    0,      // canfd_device_id
    0       // canfd_channel_id
);

if (!hand || !hand->Init()) {
    std::cerr << "Failed to initialize" << std::endl;
    return -1;
}

// Set joint angles (unit: radians, 12 joints for O12)
std::vector<double> angles(12, 0.0);  // All joints to zero position
hand->SetAllActiveJointAngles(angles);
```

### OmniHand Dex UMI

```cpp
#include "omnihand/omnihand_dex_umi.h"

// Create hand instance
auto hand = OmniHandDexUMI::createHandByZlgcan(
    HandType::LEFT,
    1,      // hand_device_id
    0,      // canfd_device_id
    0       // canfd_channel_id
);

if (!hand || !hand->Init()) {
    std::cerr << "Failed to initialize" << std::endl;
    return -1;
}

// Register position report callback
hand->SetPositionReportCallback([](const std::vector<int16_t>& positions) {
    std::cout << "Position report received: " << positions.size() << " values" << std::endl;
}, 100);  // 100 Hz frequency
```

## Related Documentation

- [SocketCAN Setup Guide](SOCKETCAN_SETUP.md) - For Linux SocketCAN configuration
- [OmniHand 2025 (O10) Kinematics Solver C++ API](API_KINEMATICS_CPP_O10.md) - For kinematics calculations
- [OmniHand Pro 2025 (O12) Kinematics Solver C++ API](API_KINEMATICS_CPP_O12.md) - For kinematics calculations
- [Server Interface API](API_SERVER.md) - WebSocket and RESTful server/client API (C++, Python, Web)
