import socket
import json
from threading import Thread
from typing import List, Dict, Optional
import math


# Glove UDP receive module
# Receives and parses sensor data over UDP for main program.

class Vector3Float:
    """
    3D float vector for finger joint samples
    """
    def __init__(self, x: float, y: float, z: float):
        self.x = x
        self.y = y
        self.z = z

class ServerStatus:
    """
    Server status enum
    """
    NO_INIT = 0     # not initialized
    READY = 1       # ready to receive
    IN_LISTENING = 2 # listening
    END = 3         # stopped

class GloveReceiver:
    """
    UDP glove receiver and parser.
    """
    def __init__(self, server_ip="192.168.5.71", port=7777):
        self.port = port  # listen port
        self.sock = None  # UDP socket
        self.server_addr = (server_ip, self.port)  # bind address
        self.name_list: List[str] = []  # role names
        self.glove_vec_res = [Vector3Float(0, 0, 0) for _ in range(30)]  # default finger samples
        self.cur_status = ServerStatus.NO_INIT  # current status
        self.recv_thread: Optional[Thread] = None  # recv thread
        self.glove_data_list: List[Dict] = []  # glove frames
        self.controller_data_list: List[Dict] = []  # controller frames

    def initialize(self):
        """
        Init UDP listener
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
        Start recv thread
        """
        if self.cur_status != ServerStatus.READY:
            print("GloveReceiver is not ready to start listening")
            return
        self.cur_status = ServerStatus.IN_LISTENING
        print("GloveReceiver Start Listening..")
        # daemon=True: do not block main exit; still call end_listening to close socket and unblock recvfrom
        self.recv_thread = Thread(target=self.recv_func, daemon=True)
        self.recv_thread.start()

    def end_listening(self):
        """
        Stop listening and join recv thread.

        Must close UDP socket while thread blocks in recvfrom to wake it;
        otherwise join hangs forever and Ctrl+C cannot exit main process.
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
        Recv loop: parse incoming datagrams
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
                # common when another thread closes sock
                break
            except Exception as e:
                if self.cur_status != ServerStatus.IN_LISTENING:
                    break
                print(f"Error receiving data: {e}")

    def process_data(self, data: str):
        """
        Parse JSON payload into glove/controller buckets
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
                    # controller keys start with l_ or r_
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
        List all role names
        """
        return [glove["roleName"] for glove in self.glove_data_list]

    def get_finger_data_for_o10hand(self, role_name: str, hand: str) -> List[float]:
        """
        Get 10-DOF joint targets for role, formatted for transmit
        """
        def to_10hand_rad(data: float, min_val: int, max_val: int) -> float:
            # clamp to min/max
            if data < min_val:
                data = min_val
            if data > max_val:
                data = max_val
            # scale to radians (* pi/180)
            x = data * math.pi / 180
            return x
        def reverse_to_10hand_rad(data: float, min_val: int, max_val: int) -> float:
            # clamp to min/max
            if data < min_val:
                data = min_val
            if data > max_val:
                data = max_val
            # scale to radians (* pi/180)
            x = data * math.pi / 180
            return -x
        def to_10hand_rad_minus(data: float, min_val: int, max_val: int) -> float:
            # invert axis
            data = data * -1
            # clamp to min/max
            if data < min_val:
                data = min_val
            if data > max_val:
                data = max_val
            # scale to radians (* pi/180)
            x = data * math.pi / 180
            return x
        def reverse_to_10hand_rad_minus(data: float, min_val: int, max_val: int) -> float:
            # invert axis
            data = data * -1
            # clamp to min/max
            if data < min_val:
                data = min_val
            if data > max_val:
                data = max_val
            # scale to radians (* pi/180)
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
