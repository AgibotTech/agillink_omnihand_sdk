# OmniHand 2025 SDK C++ API

## Overview

The OmniHand 2025 SDK provides **product-specific interfaces** for three different products:

- **OmniHand 2025 (O10)**: 10 DOF dexterous hand with 1D tactile sensors
- **OmniHand Pro 2025 (O12)**: 12 DOF dexterous hand with 3D tactile sensors
- **OmniHand Dex UMI (O10 UMI)**: 10 DOF dexterous hand with UMI protocol support

Each product has its own interface class (`OmniHand2025`, `OmniHandPro2025`, `OmniHandDexUMI`) with product-specific factory methods and APIs. This design provides better type safety and clearer API organization compared to a unified interface with `ProductType`.

## Product-Specific API Documentation

- **[OmniHand 2025 (O10) C++ API](API_CPP_O10.md)** - 10 DOF, 1D tactile sensors, supports CAN and RS485
- **[OmniHand Pro 2025 (O12) C++ API](API_CPP_O12.md)** - 12 DOF, 3D tactile sensors, CAN only
- **[OmniHand Dex UMI (O10 UMI) C++ API](API_CPP_O10_UMI.md)** - 10 DOF, UMI protocol, periodic reports, CAN only

## Common Enums and Data Structures

All products share common enums and data structures. These are documented in each product-specific API document, but here's a quick reference:

### ProductType (for reference only, not used in new API)

```cpp
enum class ProductType : unsigned char {
    OMNIHAND_2025        = 0,    // OmniHand 2025 (10 DOF)
    OMNIHAND_PRO_2025    = 1,    // OmniHand Pro 2025 (12 DOF)
    OMNIHAND_DEX_UMI     = 2,    // OmniHand Dex UMI (O10 UMI) (10 DOF, UMI protocol)
    UNKNOWN_PRODUCT_TYPE = 255   // Unknown
};
```

**Note**: The new product-specific interfaces do not require `ProductType` - each class is already typed for its product.

### EHandType

```cpp
enum class EHandType : unsigned char {
    eLeft    = 0,    // Left hand
    eRight   = 1,    // Right hand
    eUnknown = 10    // Unknown
};
```

### EFinger

```cpp
enum class EFinger : unsigned char {
    eThumb   = 0x01,    // Thumb
    eIndex   = 0x02,    // Index finger
    eMiddle  = 0x03,    // Middle finger
    eRing    = 0x04,    // Ring finger
    eLittle  = 0x05,    // Little (pinky) finger
    ePalm    = 0x06,    // Palm
    eDorsum  = 0x07,    // Dorsum (back of hand)
    eUnknown = 0xff     // Unknown
};
```

### EControlMode

```cpp
enum class EControlMode : unsigned char {
  ePosi           = 0,    // Position mode
  eServo          = 1,    // Servo mode
  eVelo           = 2,    // Velocity mode
  eTorque         = 3,    // Torque mode (Not supported: pure torque control not available)
  ePosiTorque     = 4,    // Position-Torque mode (Mixed control: position + torque)
  eVeloTorque     = 5,    // Velocity-Torque mode (Mixed control: velocity + torque)
  ePosiVeloTorque = 6,    // Position-Velocity-Torque mode (Mixed control: position + velocity + torque)
  eUnknown        = 10    // Unknown mode
};
```

## Quick Start Examples

### OmniHand 2025 (O10)

```cpp
#include "omnihand/omnihand_2025.h"

// Create hand instance
auto hand = OmniHand2025::createHandByZlgcan(
    EHandType::eLeft,
    1,      // hand_device_id
    0,      // canfd_device_id
    0       // canfd_channel_id
);

if (!hand || !hand->Init()) {
    std::cerr << "Failed to initialize" << std::endl;
    return -1;
}

// Set motor positions (range: 0-4096 for O10)
std::vector<int16_t> positions{500, 2081, 4094, 2029, 4094, 4094, 2048, 4094, 4000, 4094};
hand->SetAllJointMotorPosi(positions);
```

### OmniHand Pro 2025 (O12)

```cpp
#include "omnihand/omnihand_pro_2025.h"

// Create hand instance
auto hand = OmniHandPro2025::createHandByZlgcan(
    EHandType::eLeft,
    1,      // hand_device_id
    0,      // canfd_device_id
    0       // canfd_channel_id
);

if (!hand || !hand->Init()) {
    std::cerr << "Failed to initialize" << std::endl;
    return -1;
}

// Set motor positions (range: 0-2000 for O12)
std::vector<int16_t> positions{500, 1000, 1500, 2000, 1000, 1500, 500, 1000, 1500, 2000, 1000, 1500};
hand->SetAllJointMotorPosi(positions);
```

### OmniHand Dex UMI

```cpp
#include "omnihand/omnihand_dex_umi.h"

// Create hand instance
auto hand = OmniHandDexUMI::createHandByZlgcan(
    EHandType::eLeft,
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

## Migration from Unified Interface

If you were using the old unified `OmniHand::createHandByZlgcan(ProductType, ...)` interface:

**Old code:**
```cpp
auto hand = OmniHand::createHandByZlgcan(
    ProductType::OMNIHAND_2025,
    EHandType::eLeft,
    1, 0, 0
);
```

**New code:**
```cpp
auto hand = OmniHand2025::createHandByZlgcan(
    EHandType::eLeft,
    1, 0, 0
);
```

The product type is now determined by the class you use, providing better type safety and compile-time checks.

## Related Documentation

- [SocketCAN Setup Guide](SOCKETCAN_SETUP.md) - For Linux SocketCAN configuration
- [OmniHand 2025 (O10) Kinematics Solver C++ API](API_KINEMATICS_CPP_O10.md) - For kinematics calculations
- [OmniHand Pro 2025 (O12) Kinematics Solver C++ API](API_KINEMATICS_CPP_O12.md) - For kinematics calculations
