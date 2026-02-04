# OmniHand 2025 SDK ROS2 Interface

## Overview

The OmniHand 2025 SDK provides ROS2 interfaces for two product models:

- **OmniHand 2025 (O10)**: 10 DOF dexterous hand with 1D tactile sensors
- **OmniHand Pro 2025 (O12)**: 12 DOF dexterous hand with 3D tactile sensors

Each product has its own ROS2 node and message types, providing product-specific interfaces.

## Product-Specific ROS2 Documentation

- **[OmniHand 2025 (O10) ROS2 Interface](API_ROS2_O10.md)** - 10 DOF, 1D tactile sensors, supports mix control
- **[OmniHand Pro 2025 (O12) ROS2 Interface](API_ROS2_O12.md)** - 12 DOF, 3D tactile sensors, no mix control

## Quick Reference

### O10 Topics
- Topic prefix: `/omnihand/omnihand_2025/`
- Message namespace: `omnihand_2025_node_msgs`
- DOF: 10
- Motor position range: 0-4096
- Supports mix control: Yes

### O12 Topics
- Topic prefix: `/omnihand/omnihand_pro_2025/`
- Message namespace: `omnihand_pro_2025_node_msgs`
- DOF: 12
- Motor position range: 0-2000
- Supports mix control: No

For detailed topic lists and message definitions, please refer to the product-specific documentation above.

