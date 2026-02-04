# Copyright (c) 2025, Agibot Co., Ltd.
# OmniHand 2025 SDK is licensed under Mulan PSL v2.

"""
OmniHand Dex UMI 触觉传感器周期上报示例

此示例演示如何通过回调函数接收和处理 OmniHand Dex UMI 产品的触觉传感器周期上报数据：
- 触觉传感器周期上报（Pn6 = 0x06，频率由 Pn2.04 设置，默认 100Hz）
- 触觉传感器数据为原始传感器数据（单位：1g，最大值：255g）

注意：
1. 周期上报是设备主动发送的数据，不需要发送请求
2. 数据通过 ProcessMsg 回调函数自动接收
3. 用户注册的回调函数会在数据到达时被调用
4. 回调函数在 RecvFrame 线程中执行，需要注意线程安全

使用方法：
    python3 demo_tactile_sensor_report.py                    # 使用默认频率（100Hz）
    python3 demo_tactile_sensor_report.py --test-frequency   # 测试频率设置功能
    python3 demo_tactile_sensor_report.py --freq 50          # 设置自定义频率
"""

from omnihand import OmniHandDexUMI, EHandType, EFinger
import time
import threading
import argparse

# 用于统计和显示的数据
tactile_report_count = 0
lock = threading.Lock()

def tactile_sensor_report_callback(sensor_data, sensor_id):
    """触觉传感器周期上报回调函数"""
    global tactile_report_count
    with lock:
        tactile_report_count += 1
        if tactile_report_count % 10 == 0:  # 每10次打印一次
            print(f"\n[Tactile Sensor Report #{tactile_report_count}]")
            print(f"  Sensor ID: {sensor_id} (EFinger: {sensor_data.sensor_id})")
            print(f"  Data points: {len(sensor_data.data)}")
            if len(sensor_data.data) > 0:
                print(f"  First 10 values: {sensor_data.data[:10]}")
                print(f"  Sum: {sum(sensor_data.data)} g")

def main():
    # 解析命令行参数
    parser = argparse.ArgumentParser(
        description='OmniHand Dex UMI Tactile Sensor Report Demo',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  python3 demo_tactile_sensor_report.py                    # 使用默认频率（100Hz）
  python3 demo_tactile_sensor_report.py --test-frequency   # 测试频率设置功能
  python3 demo_tactile_sensor_report.py --freq 50         # 设置自定义频率
        """
    )
    parser.add_argument('--test-frequency', action='store_true',
                       help='Test frequency setting functions before registering callback')
    parser.add_argument('--freq', type=int, default=None,
                       help='Tactile sensor report frequency in Hz (default: 100)')
    
    args = parser.parse_args()
    
    print("=" * 60)
    print("OmniHand Dex UMI Tactile Sensor Report Demo")
    print("=" * 60)
    print("Protocol: Pn6 (0x06) - Tactile Sensor (read-only, periodic report)")
    print("Frequency: Pn2.04 (default: 100Hz)")
    print("=" * 60)
    
    # 创建 OmniHand Dex UMI 灵巧手实例
    hand = OmniHandDexUMI.create_hand_by_zlgcan(
        hand_type=EHandType.LEFT,
        hand_device_id=1,
        canfd_device_id=0,
        canfd_channel_id=0
    )
    
    # 检查初始化状态
    if not hand.init():
        print("[Error]: Failed to initialize OmniHand Dex UMI hand!")
        return
    
    print("[OK]: OmniHand Dex UMI hand initialized successfully!\n")
    
    # 获取当前设备信息，查看默认频率
    device_info = hand.get_device_info()
    default_tactile_freq = device_info.tactile_sensor_report_frequency if device_info.tactile_sensor_report_frequency is not None else 100
    
    print("=" * 60)
    print("Current Tactile Sensor Report Frequency (Pn2.04):")
    print(f"  Frequency: {default_tactile_freq} Hz")
    print("=" * 60)
    
    # 如果指定了 --test-frequency，测试频率设置功能
    if args.test_frequency:
        # 测试方式1：使用独立的频率设置函数
        print("\n[Test 1]: Setting frequency using separate function...")
        test_freq = 20  # 20Hz
        
        print(f"Setting tactile sensor report frequency to {test_freq} Hz...")
        hand.set_tactile_sensor_report_frequency(test_freq)
        
        # 验证频率设置
        device_info_after = hand.get_device_info()
        print(f"\nVerification:")
        if device_info_after.tactile_sensor_report_frequency is not None:
            print(f"  Tactile sensor report frequency: {device_info_after.tactile_sensor_report_frequency} Hz")
        
        # 测试方式2：在注册回调时同时设置频率
        print("\n[Test 2]: Registering callback with frequency parameter...")
        # 恢复到100Hz并同时注册回调
        print("Registering callback with frequency=100Hz...")
        hand.set_tactile_sensor_report_callback(tactile_sensor_report_callback, frequency=100)
        
        # 验证频率
        device_info_final = hand.get_device_info()
        print(f"\nFinal frequency after callback registration:")
        if device_info_final.tactile_sensor_report_frequency is not None:
            print(f"  Tactile sensor report frequency: {device_info_final.tactile_sensor_report_frequency} Hz")
        
        print("\n" + "=" * 60)
    else:
        # 如果指定了自定义频率，使用命令行参数设置
        if args.freq is not None:
            print(f"\nSetting tactile sensor report frequency to {args.freq} Hz...")
            hand.set_tactile_sensor_report_frequency(args.freq)
            
            # 验证频率设置
            device_info_set = hand.get_device_info()
            print(f"\nCurrent frequency:")
            if device_info_set.tactile_sensor_report_frequency is not None:
                print(f"  Tactile sensor report frequency: {device_info_set.tactile_sensor_report_frequency} Hz")
            print()
        
        # 注册回调函数（如果指定了频率参数，在注册时使用）
        freq = args.freq if args.freq is not None else None
        
        if freq is not None:
            print("Registering callback with specified frequency...")
            hand.set_tactile_sensor_report_callback(tactile_sensor_report_callback, frequency=freq)
        else:
            print("Registering callback with default frequency...")
            hand.set_tactile_sensor_report_callback(tactile_sensor_report_callback)
    
    print("\nCallback registered. Receiving tactile sensor reports...")
    print("Press Ctrl+C to stop...\n")
    
    try:
        # 主线程等待，回调函数在后台线程中执行
        while True:
            time.sleep(1.0)
            # 可以在这里做其他事情，比如显示统计信息
            with lock:
                if tactile_report_count > 0:
                    print(f"[Stats] Tactile sensor reports received: {tactile_report_count}", end='\r')
            
    except KeyboardInterrupt:
        print("\n\n[Info]: Stopped by user")
    
    # 取消注册回调函数
    print("\nUnregistering callback...")
    hand.set_tactile_sensor_report_callback(None)
    
    print("\n[Done]: Tactile sensor report demo completed!")
    print(f"Total tactile sensor reports received: {tactile_report_count}")

if __name__ == "__main__":
    main()
