"""Shared helpers for std_msgs Int8MultiArray / Int16MultiArray (1-D layout)."""

from std_msgs.msg import Int8MultiArray, Int16MultiArray, MultiArrayDimension, MultiArrayLayout


def make_int8_multi_array(values: list) -> Int8MultiArray:
    msg = Int8MultiArray()
    msg.layout = MultiArrayLayout()
    msg.layout.data_offset = 0
    if values:
        msg.layout.dim.append(
            MultiArrayDimension(label='joints', size=len(values), stride=len(values))
        )
    msg.data = [int(v) for v in values]
    return msg


def make_int16_multi_array(values: list) -> Int16MultiArray:
    msg = Int16MultiArray()
    msg.layout = MultiArrayLayout()
    msg.layout.data_offset = 0
    if values:
        msg.layout.dim.append(
            MultiArrayDimension(label='joints', size=len(values), stride=len(values))
        )
    msg.data = [int(v) for v in values]
    return msg
