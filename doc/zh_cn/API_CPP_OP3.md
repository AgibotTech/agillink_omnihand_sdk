# OmniPicker 3 (OP3) C++ API

## 概述

**OmniPicker 3 (OP3)** 为 1 自由度夹爪，通过 `OmniPicker3` 类提供 CAN 接口（与 O10/O12 相同的工厂创建方式，见 `omni_picker_3.h`）。

## 混合控制

OP3 在总线混合控制协议上仅实现 **位置 + 力矩（`MixControlMode::POSITION_TORQUE`）**，力矩字段为 **int16**（与 O12 相同，单位 0.01N），单帧步长为 5 字节。

基类 `OmniHand` 还提供 `MixControlByPV`、`MixControlByPVT` 接口；在 OP3 上调用将打印错误并返回空向量，**不会下发 CAN 指令**。

| 接口 | 支持情况 |
|------|----------|
| `MixControlByPT(positions, torques)` | 支持 |
| `MixControlByPV(velocities, torques)` | **不支持** |
| `MixControlByPVT(positions, velocities, torques)` | **不支持** |

```cpp
#include "omnihand/omni_picker_3.h"

// positions / torques 长度按关节数（OP3 为 1）；数组下标 i 对应混合控制关节 id i（0-based）
std::vector<MixCtrl> echo = hand->MixControlByPT({pos}, {torque});

// 以下在 OP3 上不可用，返回 {}
hand->MixControlByPV({vel}, {torque});
hand->MixControlByPVT({pos}, {vel}, {torque});
```

回读 echo 中 `joint_index_` 为 **1-based**（与 O10/O12 混合控制回读一致）。
