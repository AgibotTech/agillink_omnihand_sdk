"""
agi_o10_hand Glove → OmniHand O10 HTTP bridge.

Provides two HTTP interfaces, following AimDK hal style:

1) Get current O10 hand state from glove:

   POST /rpc/agi.o10hand.GloveControlService/GetO10HandState

   Request:
   {
     "data": {
       "role_name": "Player1",
       "hand": "right"   // "left" | "right"
     }
   }

   Response:
   {
     "data": {
       "role_name": "Player1",
       "hand": "right",
       "active_joint_pos": [...],   // 10 joint angles, rad
       "actuator_input":   [...]    // 10 actuator inputs, 0~4096
     }
   }

2) Drive O10 hand directly using current glove pose:

   POST /rpc/agi.o10hand.GloveControlService/SetO10HandCommandFromGlove

   Request:
   {
     "data": {
       "role_name": "Player1",
       "hand": "right",
       "scale": 0.8      // optional, scale motion amplitude
     }
   }

   Response:
   {
     "data": {
       "success": true,
       "message": "Command sent",
       "active_joint_pos": [...],
       "actuator_input":   [...]
     }
   }

Run service example:

  uvicorn agi_o10_hand_service:app --host 0.0.0.0 --port 56421
"""

import json
import subprocess
from typing import List, Dict, Any, Optional

try:
    from omnihand.omnihand_2025 import OmniHand2025Solver
    _SOLVER_AVAILABLE = True
except ImportError:
    _SOLVER_AVAILABLE = False
    OmniHand2025Solver = None  # type: ignore

# Cache solver instances for left and right hands
_solver_cache: Dict[bool, Any] = {True: None, False: None}

# HAL service address (can be configured via environment variables)
HAL_HOST = "127.0.0.1"
HAL_PORT = 56421


def _bool_is_left(hand: str) -> bool:
    """Check if hand is left hand"""
    return hand.lower() == "left"


def _actuator_to_o10_finger_pos(actuator_input: List[int]) -> Dict[str, int]:
    """
    Convert actuator input to O10FingerPos format.
    
    Note: O10 motor input range is 0-4096 (different from O12 which is 0-2000).
    
    Mapping:
    - actuator_input[0] -> thumb_roration_pos_0
    - actuator_input[1] -> thumb_wiggles_pos_1
    - actuator_input[2] -> thumb_bent_pos_2
    - actuator_input[3] -> index_wiggles_pos_0
    - actuator_input[4] -> index_bent_pos_1
    - actuator_input[5] -> middle_bent_pos
    - actuator_input[6] -> ring_wiggles_pos_0
    - actuator_input[7] -> ring_bent_pos_1
    - actuator_input[8] -> pinky_wiggles_pos_0
    - actuator_input[9] -> pinky_bent_pos_1
    """
    if len(actuator_input) != 10:
        raise ValueError(f"expected 10 actuator inputs, got {len(actuator_input)}")
    
    return {
        "thumb_roration_pos_0": actuator_input[0],
        "thumb_wiggles_pos_1": actuator_input[1],
        "thumb_bent_pos_2": actuator_input[2],
        "index_wiggles_pos_0": actuator_input[3],
        "index_bent_pos_1": actuator_input[4],
        "middle_bent_pos": actuator_input[5],
        "ring_wiggles_pos_0": actuator_input[6],
        "ring_bent_pos_1": actuator_input[7],
        "pinky_wiggles_pos_0": actuator_input[8],
        "pinky_bent_pos_1": actuator_input[9],
    }


def _send_hand_command_to_hal(
    hand_str: str,
    actuator_input: List[int],
    hal_host: str | None = None,
    hal_port: int | None = None,
) -> Dict[str, Any]:
    """
    Send hand control command to HAL service via curl
    
    Args:
        hand_str: "left" or "right"
        actuator_input: 10 actuator input values (0~4096)
        hal_host: HAL service IP (defaults to HAL_HOST)
        hal_port: HAL service port (defaults to HAL_PORT)
    
    Returns:
        curl command response result
    """
    hand_key = "left" if hand_str.lower() == "left" else "right"

    if hal_host is None:
        hal_host = HAL_HOST
    if hal_port is None:
        hal_port = HAL_PORT
    
    # Convert to O10FingerPos format
    finger_pos = _actuator_to_o10_finger_pos(actuator_input)
    
    # Build request JSON
    request_data = {
        "data": {
            hand_key: {
                "agi_o10_hand": {
                    "finger": {
                        "pos": finger_pos
                    }
                }
            }
        }
    }
    
    # Call curl to send command
    url = f"http://{hal_host}:{hal_port}/rpc/aimdk.protocol.HalHandService/SetHandCommand"
    curl_cmd = [
        "curl",
        "-i",
        "-H", "content-type:application/json",
        "-H", "timeout: 60000",
        "-X", "POST",
        url,
        "-d", json.dumps(request_data)
    ]
    
    try:
        result = subprocess.run(
            curl_cmd,
            capture_output=True,
            text=True,
            timeout=5.0
        )
        if result.returncode != 0:
            raise RuntimeError(f"curl failed: {result.stderr}")
        return {"success": True, "stdout": result.stdout}
    except subprocess.TimeoutExpired:
        raise RuntimeError("curl command timeout")
    except Exception as e:
        raise RuntimeError(f"failed to send command to HAL: {e}") from e


def _get_hand_state_from_hal(
    hal_host: str | None = None,
    hal_port: int | None = None,
) -> Dict[str, Any]:
    """
    Get current hand state from HAL service via curl (aimdk.protocol.HalHandService/GetHandState).

    Note:
      - This simply returns the raw response text from HAL, without parsing proto structure.
    """
    if hal_host is None:
        hal_host = HAL_HOST
    if hal_port is None:
        hal_port = HAL_PORT

    url = f"http://{hal_host}:{hal_port}/rpc/aimdk.protocol.HalHandService/GetHandState"
    curl_cmd = [
        "curl",
        "-i",
        "-H", "content-type:application/json",
        "-H", "timeout: 60000",
        "-X", "POST",
        url,
        "-d", "{}",
    ]

    try:
        result = subprocess.run(
            curl_cmd,
            capture_output=True,
            text=True,
            timeout=5.0,
        )
        if result.returncode != 0:
            raise RuntimeError(f"curl failed: {result.stderr}")
        return {"success": True, "raw_response": result.stdout}
    except subprocess.TimeoutExpired:
        raise RuntimeError("curl command timeout")
    except Exception as e:  # noqa: BLE001
        raise RuntimeError(f"failed to get hand state from HAL: {e}") from e


def _get_solver(is_left_hand: bool) -> OmniHand2025Solver:
    """
    Get or create solver instance for the specified hand type.
    Uses caching to avoid recreating solvers.
    
    Args:
        is_left_hand: True for left hand, False for right hand
    
    Returns:
        OmniHand2025Solver instance
    """
    if not _SOLVER_AVAILABLE:
        raise RuntimeError(
            "C++ kinematics solver not available. "
            "Please rebuild and reinstall the omnihand package."
        )
    
    if _solver_cache[is_left_hand] is None:
        _solver_cache[is_left_hand] = OmniHand2025Solver(hand_type=is_left_hand)
    
    return _solver_cache[is_left_hand]


def set_hand_position(
    hand_type: str,
    positions: List[float],
    hal_host: str | None = None,
    hal_port: int | None = None,
) -> None:
    """
    Set hand position (via HAL service)
    
    Args:
        hand_type: "left" or "right"
        positions: Joint angle list (rad, length 10)
        hal_host: HAL service IP (defaults to HAL_HOST)
        hal_port: HAL service port (defaults to HAL_PORT)
    
    Process:
        1. Use C++ kinematics solver to convert joint angles to actuator inputs
        2. Call HAL's SetHandCommand interface via curl
    """
    if len(positions) != 10:
        raise ValueError(f"expected 10 joint positions, got {len(positions)}")
    
    # 1. Get solver instance and convert joint angles -> actuator inputs
    is_left = _bool_is_left(hand_type)
    solver = _get_solver(is_left)
    actuator_input: List[int] = solver.active_joint_pos_to_actuator_input(positions)
    
    # 2. Send command to HAL service via curl
    _send_hand_command_to_hal(hand_type, actuator_input, hal_host, hal_port)
