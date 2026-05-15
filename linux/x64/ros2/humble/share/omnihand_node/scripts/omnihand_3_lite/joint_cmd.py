import time
import sys
import rclpy
from rclpy.node import Node
from sensor_msgs.msg import JointState

NUM_JOINTS = 4


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

        self.get_logger().info(
            f"{product}/{hand_side} joint_cmd started (H3L, f{NUM_JOINTS} DOF, position in rad)"
        )
    
    def _make_msg(self, positions_rad: list[float]) -> JointState:
        msg = JointState()
        msg.header.stamp = self.get_clock().now().to_msg()
        msg.position = positions_rad.copy()
        return msg

    def publish_joint_cmd(self):
        pose_open = [0.0] * NUM_JOINTS
        pose_close = [0.6] * NUM_JOINTS
        self.publisher.publish(self._make_msg(pose_open))
        time.sleep(0.5)
        self.publisher.publish(self._make_msg(pose_close))

    
    def callback(self, msg:JointState) -> None:
        self.get_logger().info(
            f'{self.product}/{self.hand_side} joint_states '
            f'(stamp={msg.header.stamp.sec}.{msg.header.stamp.nanosec:09d}, rad): '
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