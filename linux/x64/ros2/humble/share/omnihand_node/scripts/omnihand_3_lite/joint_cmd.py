import time
import sys
import rclpy
from rclpy.node import Node
from sensor_msgs.msg import JointState

NUM_JOINTS = 4
ACTUATOR_MAX = 4095

# Right-hand reference (OmniHand3LiteSolver::SetHandGesture, before left mirror).
_POSE_OPEN_R = [4095, 4095, 4095, 4095]
_POSE_CLOSE_R = [1500, 1500, 2900, 400]  # FIST


def _mirror_joints_0_and_3(motor: list[int]) -> list[int]:
    """Same as solver: left hand mirrors motor 0 and 3."""
    out = motor.copy()
    out[0] = ACTUATOR_MAX - out[0]
    out[3] = ACTUATOR_MAX - out[3]
    return out


POSE_OPEN = {
    'right': _POSE_OPEN_R,
    'left': _mirror_joints_0_and_3(_POSE_OPEN_R),
}
POSE_CLOSE = {
    'right': _POSE_CLOSE_R,
    'left': _mirror_joints_0_and_3(_POSE_CLOSE_R),
}


class JointCmdNode(Node):
    def __init__(self, hand_side: str, product: str):
        super().__init__(f'{product}_{hand_side}_joint_cmd')

        self.hand_side = hand_side
        self.product = product

        self.publisher = self.create_publisher(
            msg_type=JointState,
            topic=f"/{product}/{hand_side}/joint_cmd",
            qos_profile=10
        )

        self.subscription = self.create_subscription(
            msg_type=JointState,
            topic=f"/{product}/{hand_side}/joint_states",
            callback=self.callback,
            qos_profile=10
        )

        self.timer = self.create_timer(1.5, self.publish_joint_cmd)

        if hand_side not in POSE_OPEN:
            raise ValueError(f"hand_side must be 'left' or 'right', got {hand_side!r}")

        self.get_logger().info(
            f"{product}/{hand_side} joint_cmd started "
            f"(H3L, {NUM_JOINTS} DOF, position = motor tick 0~{ACTUATOR_MAX})"
        )
    
    def _make_msg(self, positions: list[int]) -> JointState:
        msg = JointState()
        msg.header.stamp = self.get_clock().now().to_msg()
        msg.position = [float(p) for p in positions]
        return msg

    def publish_joint_cmd(self):
        side = self.hand_side
        pose_open = POSE_OPEN[side]
        pose_close = POSE_CLOSE[side]
        self.publisher.publish(self._make_msg(pose_open))
        time.sleep(0.5)
        self.publisher.publish(self._make_msg(pose_close))

    
    def callback(self, msg:JointState) -> None:
        self.get_logger().info(
            f'{self.product}/{self.hand_side} joint_states '
            f'(stamp={msg.header.stamp.sec}.{msg.header.stamp.nanosec:09d}, tick): '
            f'{[round(p, 3) for p in msg.position]}')

def main(args=None):
    rclpy.init(args=args)
    hand_side = sys.argv[1].lower() if len(sys.argv) > 1 else 'left'
    product = sys.argv[2].lower() if len(sys.argv) > 2 else 'h3l'
    node = JointCmdNode(hand_side, product)
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        try:
            rclpy.shutdown()
        except Exception:
            pass

if __name__ == "__main__":
    main()