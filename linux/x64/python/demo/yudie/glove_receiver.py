import socket
import json
from threading import Thread
from typing import List, Dict, Optional
import math


# 手套数据接收模块
# 负责通过UDP接收传感器数据并解析，供主程序调用。

class Vector3Float:
    """
    三维浮点向量类，表示手指关节数据
    """
    def __init__(self, x: float, y: float, z: float):
        self.x = x
        self.y = y
        self.z = z

class ServerStatus:
    """
    服务状态枚举类
    """
    NO_INIT = 0     # 未初始化
    READY = 1       # 已初始化，准备接收
    IN_LISTENING = 2 # 正在监听
    END = 3         # 已停止

class GloveReceiver:
    """
    手套数据接收类，负责UDP监听和数据解析。
    """
    def __init__(self, server_ip="192.168.5.71", port=7777):
        self.port = port  # 监听端口
        self.sock = None  # UDP套接字
        self.server_addr = (server_ip, self.port)  # 监听地址
        self.name_list: List[str] = []  # 角色名列表
        self.glove_vec_res = [Vector3Float(0, 0, 0) for _ in range(30)]  # 默认手指数据
        self.cur_status = ServerStatus.NO_INIT  # 当前状态
        self.recv_thread: Optional[Thread] = None  # 接收线程
        self.glove_data_list: List[Dict] = []  # 手套数据列表
        self.controller_data_list: List[Dict] = []  # 控制器数据列表

    def initialize(self):
        """
        初始化UDP监听服务
        """
        try:
            self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            self.sock.bind(self.server_addr)
            self.sock.settimeout(2)
            self.cur_status = ServerStatus.READY
            print("GloveReceiver Initialized and Ready")
        except Exception as e:
            print(f"Failed to initialize: {e}")
            self.cur_status = ServerStatus.NO_INIT

    def start_listening(self):
        """
        启动数据监听线程
        """
        if self.cur_status != ServerStatus.READY:
            print("GloveReceiver is not ready to start listening")
            return
        self.cur_status = ServerStatus.IN_LISTENING
        print("GloveReceiver Start Listening..")
        # daemon=True：主线程退出时不无限等待；仍须调用 end_listening 关闭套接字以唤醒 recvfrom
        self.recv_thread = Thread(target=self.recv_func, daemon=True)
        self.recv_thread.start()

    def end_listening(self):
        """
        停止监听并结束接收线程。

        必须先结束状态并关闭 UDP 套接字，使阻塞在 recvfrom 上的线程被唤醒；
        否则仅 join 会永久等待，Ctrl+C 后主进程无法退出。
        """
        self.cur_status = ServerStatus.END
        if self.sock is not None:
            try:
                self.sock.close()
            except OSError:
                pass
            self.sock = None
        if self.recv_thread is not None and self.recv_thread.is_alive():
            self.recv_thread.join(timeout=3.0)
        print("GloveReceiver Stopped Listening")

    def recv_func(self):
        """
        接收数据主循环，收到数据后解析
        """
        while self.cur_status == ServerStatus.IN_LISTENING:
            if self.sock is None:
                break
            try:
                data, addr = self.sock.recvfrom(1024 * 1024)
                self.process_data(data.decode("utf-8"))
            except socket.timeout:
                continue
            except OSError:
                # 其它线程调用 sock.close() 退出时常见
                break
            except Exception as e:
                if self.cur_status != ServerStatus.IN_LISTENING:
                    break
                print(f"Error receiving data: {e}")

    def process_data(self, data: str):
        """
        解析收到的JSON字符串，分离手套和控制器数据
        """
        try:
            value = json.loads(data)
            self.glove_data_list.clear()
            self.controller_data_list.clear()
            for role_name, device in value.items():
                glove_data = {"roleName": role_name, "handDatas": {}}
                controller_data = {"roleName": role_name, "controllerDatas": {}}
                parameters = device.get("Parameter", [])
                for param in parameters:
                    name = param["Name"]
                    value = (param["Value"]) if "Value" in param else 0.0
                    # 控制器数据以 l_ 或 r_ 开头
                    if name[1] == '_' and (name[0] == 'l' or name[0] == 'r'):
                        controller_data["controllerDatas"][name] = value
                    else:
                        glove_data["handDatas"][name] = value
                self.glove_data_list.append(glove_data)
                self.controller_data_list.append(controller_data)
        except Exception as e:
            print(f"Error processing data: {e}")

    def get_role_name_list(self) -> List[str]:
        """
        获取所有角色名列表
        """
        return [glove["roleName"] for glove in self.glove_data_list]

    def get_finger_data_for_o10hand(self, role_name: str, hand: str) -> List[float]:
        """
        获取指定角色的10自由度机械手关节数据，转换为发送格式
        """
        def to_10hand_rad(data: float, min_val: int, max_val: int) -> float:
            # 按照最小到最大值范围裁剪
            if data < min_val:
                data = min_val
            if data > max_val:
                data = max_val
            # 数据归一化到弧度制，乘以pi/180
            x = data * math.pi / 180
            return x
        def reverse_to_10hand_rad(data: float, min_val: int, max_val: int) -> float:
            # 按照最小到最大值范围裁剪
            if data < min_val:
                data = min_val
            if data > max_val:
                data = max_val
            # 数据归一化到弧度制，乘以pi/180
            x = data * math.pi / 180
            return -x
        def to_10hand_rad_minus(data: float, min_val: int, max_val: int) -> float:
            # 数据翻转
            data = data * -1
            # 按照最小到最大值范围裁剪
            if data < min_val:
                data = min_val
            if data > max_val:
                data = max_val
            # 数据归一化到弧度制，乘以pi/180
            x = data * math.pi / 180
            return x
        def reverse_to_10hand_rad_minus(data: float, min_val: int, max_val: int) -> float:
            # 数据翻转
            data = data * -1
            # 按照最小到最大值范围裁剪
            if data < min_val:
                data = min_val
            if data > max_val:
                data = max_val
            # 数据归一化到弧度制，乘以pi/180
            x = data * math.pi / 180
            return -x
        for glove in self.glove_data_list:
            if glove["roleName"] == role_name:
                hand_data = glove["handDatas"]
                # print(hand_data)
                if hand == 'left':
                    return [
                        reverse_to_10hand_rad(hand_data.get("l20", 0), -10, 50),
                        reverse_to_10hand_rad(hand_data.get("l2", 0), -100, 0),
                        reverse_to_10hand_rad_minus(hand_data.get("l1", 0), 0, 49),
                        reverse_to_10hand_rad(hand_data.get("l7", 0), -12, 0),
                        to_10hand_rad_minus(hand_data.get("l6", 0), 0, 90),
                        to_10hand_rad_minus(hand_data.get("l10", 0), 0, 90),
                        reverse_to_10hand_rad(hand_data.get("l15", 0), 0, 10),
                        to_10hand_rad_minus(hand_data.get("l14", 0), 0, 90),
                        reverse_to_10hand_rad(hand_data.get("l19", 0), 0, 10),
                        to_10hand_rad_minus(hand_data.get("l18", 0), 0, 90),
                    ]
                else:
                    return [
                        to_10hand_rad(hand_data.get("r20", 0), -10, 50),
                        to_10hand_rad(hand_data.get("r2", 0), -100, 0),
                        to_10hand_rad_minus(hand_data.get("r1", 0), 0, 49),
                        to_10hand_rad(hand_data.get("r7", 0), -12, 0),
                        to_10hand_rad_minus(hand_data.get("r6", 0), 0, 90),
                        to_10hand_rad_minus(hand_data.get("r10", 0), 0, 90),
                        to_10hand_rad(hand_data.get("r15", 0), 0, 10),
                        to_10hand_rad_minus(hand_data.get("r14", 0), 0, 90),
                        to_10hand_rad(hand_data.get("r19", 0), 0, 10),
                        to_10hand_rad_minus(hand_data.get("r18", 0), 0, 90),
                    ]
        return []
