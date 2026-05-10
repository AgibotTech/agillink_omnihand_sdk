# Python 单元测试

本目录包含 AGILINK OmniHand SDK Python 接口的单元测试，使用 pytest 框架。

**说明：** O10 / Dex UMI 工厂参数中的 `hand_device_id`、`canfd_device_id`、`canfd_channel_id` 在 Python 中为 `int`，绑定层传入 C++ 时为 `uint8_t`（见 `release/doc/zh_cn/API_PYTHON_O10.md`）。

## 要求

安装 pytest：

```bash
pip install pytest
```

## 运行测试

### 运行所有测试

```bash
cd python/test
pytest -v
```

### 运行特定产品的测试

```bash
# O10 测试（直接使用 pytest）
pytest -v test_omnihand_2025.py

# O10 测试（直接使用 Python 运行 - 支持 -f 参数）
python3 test_omnihand_2025.py -f 0

# O12 测试（直接使用 pytest）
pytest -v test_omnihand_pro_2025_pytest.py

# O12 测试（直接使用 Python 运行 - 支持 -f 参数）
python3 test_omnihand_pro_2025_pytest.py -f 0

# UMI 测试（直接使用 pytest）
pytest -v test_omnihand_dex_umi.py

# UMI 测试（直接使用 Python 运行）
python3 test_omnihand_dex_umi.py
```

### 使用请求间隔参数运行（仅 O10 和 O12）

```bash
# O10 测试，自定义间隔（例如 0ms = 无限制，5ms = 默认值）
python3 test_omnihand_2025.py -f 0

# O12 测试，自定义间隔（例如 0ms = 无限制，5ms = 默认值）
python3 test_omnihand_pro_2025_pytest.py -f 0
```

**注意**：
- 直接使用 Python 运行时，`-f` 参数指定请求间隔（毫秒，0-100ms）。使用 `0` 表示禁用间隔限制。
- 直接使用 `pytest` 时，使用环境变量：`OMNIHAND_REQUEST_INTERVAL=0 pytest -v test_omnihand_2025.py`
- UMI 测试不支持 `-f` 间隔参数，因为 UMI 协议使用固定的周期上报。
- 直接使用 Python 运行测试会自动启用详细模式（`-v`）以显示详细的测试结果。

### 运行特定测试

```bash
# 运行特定的测试函数
pytest -v test_omnihand_2025.py::test_get_vendor_info

# 运行匹配模式的测试
pytest -v -k "tactile"
```

### 详细输出

```bash
pytest -v
```

### 显示 print 语句

```bash
pytest -s
```

## 测试结构

测试组织方式与 C++ gtest 结构类似：

- **test_omnihand_2025.py**：OmniHand 2025 (O10, 10 DOF) 的测试
- **test_omnihand_pro_2025.py**：OmniHand Pro 2025 (O12, 12 DOF) 的测试
- **test_omnihand_dex_umi.py**：OmniHand Dex UMI (UMI 协议) 的测试

## 测试覆盖

每个测试文件涵盖：

- 工厂方法（create_hand）
- 初始化
- 厂商和设备信息
- 设备 ID 设置（带适当的清理）
- 关节角度控制
- 控制模式（只读）
- 触觉传感器（产品特定）
- 错误报告
- 温度报告
- 电流报告
- 运动学求解器
- UMI 特定功能（回调、上报频率）

## 注意事项

- 测试需要连接硬件
- 如果设备初始化失败，测试将跳过
- 测试优雅地处理超时（跳过而不是失败）

## 更多信息

- [English Documentation](README.md)
