import time
import sys
import rclpy
from rclpy.node import Node
from sensor_msgs.msg import JointState

NUM_JOINTS = 4

# Motor tick per joint [axis_1, axis_2, axis_3, axis_4], range 0~4095.
# From OmniHand3LiteSolver::SetHandGesture (OPEN / FIST); left/right listed separately.
POSE_OPEN = {
  # OMNI_HAND_3_LITE_GESTURE_OPEN
    'right': [4095, 4095, 4095, 4095],
    'left': [0, 4095, 4095, 0],
}
POSE_CLOSE = {
  # OMNI_HAND_3_LITE_GESTURE_FIST
    'right': [1500, 1500, 2900, 400],
    'left': [2595, 1500, 2900, 3695],
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
            f"(H3L, {NUM_JOINTS} DOF, position = motor tick 0~4095)"
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