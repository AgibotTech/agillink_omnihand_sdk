# OmniHand 2025 SDK

[English Documentation](README.md)

## 快速链接

- 📖 **[快速入门指南](doc/zh_cn/QUICK_START.md)** - 5 分钟上手
- 🔧 **[故障排除](doc/zh_cn/TROUBLESHOOTING.md)** - 常见问题和解决方案

## 产品概述

OmniHand 2025 SDK 支持三种产品型号：

**OmniHand 2025 灵动款 (O10)**：紧凑型高自由度交互灵巧手，具有 `10 个主动 + 6 个被动自由度`。重量仅 500g，采用 CANFD 通信接口，配备 `400+ 触觉点，0.1N 阵列分辨率，最大指尖力 5N`。适用于各种人形机器人和机械臂。其紧凑轻量化的设计和丰富的触觉交互能力，使其在交互服务、研究教育、轻量作业等领域具有重要价值。

![](doc/pic/hand_o10.jpg)

**OmniHand Pro 2025 专业款 (O12)**：12 自由度专业灵巧手，具有精确操作和灵活控制能力。配备触觉传感器和多种控制模式（位置控制、力矩控制、混合控制），适用于研究教育、娱乐商业演出、展览引导、工业场景等多种应用。

![](doc/pic/hand_o12.jpg)

**OmniHand Dex UMI (O10 UMI)**：使用 UMI 协议的只读灵巧手，支持周期性的位置和触觉传感器数据上报。

## 灵巧手电机索引

**OmniHand 2025 灵动款 (O10)**：具有 10 个自由度，索引从 1 到 10。对应的控制电机如下图所示：

![](doc/pic/hand_o10_motors.jpg)

**OmniHand Pro 2025 专业款 (O12)**：具有 12 个自由度，索引从 1 到 12。对应的控制电机如下图所示：

![](doc/pic/hand_o12_motors.jpg)

## 平台文档

- **[Linux (x64)](linux/x64/README_zh_cn.md)** - 安装、USB 配置、ROS2
- **[Windows (x64)](windows/README_zh_cn.md)** - 安装、驱动配置

## API 文档

### C++ API
- [C++ API 索引](doc/zh_cn/API_CPP.md)
- [OmniHand 2025 (O10) C++ API](doc/zh_cn/API_CPP_O10.md)
- [OmniHand Pro 2025 (O12) C++ API](doc/zh_cn/API_CPP_O12.md)
- [OmniHand Dex UMI (O10 UMI) C++ API](doc/zh_cn/API_CPP_O10_UMI.md)

### Python API
- [Python API 索引](doc/zh_cn/API_PYTHON.md)
- [OmniHand 2025 (O10) Python API](doc/zh_cn/API_PYTHON_O10.md)
- [OmniHand Pro 2025 (O12) Python API](doc/zh_cn/API_PYTHON_O12.md)
- [OmniHand Dex UMI (O10 UMI) Python API](doc/zh_cn/API_PYTHON_UMI.md)

### 运动学 API
- [OmniHand 2025 (O10) 运动学 C++](doc/zh_cn/API_KINEMATICS_CPP_O10.md)
- [OmniHand 2025 (O10) 运动学 Python](doc/zh_cn/API_KINEMATICS_PYTHON_O10.md)
- [OmniHand Pro 2025 (O12) 运动学 C++](doc/zh_cn/API_KINEMATICS_CPP_O12.md)
- [OmniHand Pro 2025 (O12) 运动学 Python](doc/zh_cn/API_KINEMATICS_PYTHON_O12.md)

### ROS2 接口（仅 Linux）
- [ROS2 API 索引](doc/zh_cn/API_ROS2.md)
- [OmniHand 2025 (O10) ROS2 接口](doc/zh_cn/API_ROS2_O10.md)
- [OmniHand Pro 2025 (O12) ROS2 接口](doc/zh_cn/API_ROS2_O12.md)

## 许可证

Mulan PSL v2 - Copyright (c) 2025, Agibot Co., Ltd.
