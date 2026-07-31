"""
Minimal protobuf definitions for `RetargetInfo`.

Compatible with `hand_send_data` UDP protobuf format (protobuf 6+ API).
"""

from __future__ import annotations

from google.protobuf import descriptor_pb2, descriptor_pool

from ..dynamic_util import get_message_class


_LEFT_GROUP_ID = 1
_RIGHT_GROUP_ID = 2

_ABS_JOINT_CONTROL_TYPE = 1


def _build_messages():
    file_proto = descriptor_pb2.FileDescriptorProto()
    file_proto.name = "genie_msgs_pb/msg/RetargetInfo.proto"
    file_proto.package = "genie_msgs_pb.msg"
    file_proto.syntax = "proto3"

    retarget_info = file_proto.message_type.add()
    retarget_info.name = "RetargetInfo"

    group_id = retarget_info.enum_type.add()
    group_id.name = "GroupId"
    # proto3: first enum value must be 0 (wire values 1,2 unchanged for sender compat)
    group_id.value.add(name="GROUP_UNSPECIFIED", number=0)
    group_id.value.add(name="GROUP_LEFT_TOOL", number=_LEFT_GROUP_ID)
    group_id.value.add(name="GROUP_RIGHT_TOOL", number=_RIGHT_GROUP_ID)

    control_type = retarget_info.enum_type.add()
    control_type.name = "ControlType"
    control_type.value.add(name="CONTROL_UNSPECIFIED", number=0)
    control_type.value.add(
        name="ABS_JOINT",
        number=_ABS_JOINT_CONTROL_TYPE,
    )

    f_group_id = retarget_info.field.add()
    f_group_id.name = "group_id"
    f_group_id.number = 1
    f_group_id.label = descriptor_pb2.FieldDescriptorProto.LABEL_OPTIONAL
    f_group_id.type = descriptor_pb2.FieldDescriptorProto.TYPE_ENUM
    f_group_id.type_name = ".genie_msgs_pb.msg.RetargetInfo.GroupId"

    f_control_type = retarget_info.field.add()
    f_control_type.name = "control_type"
    f_control_type.number = 2
    f_control_type.label = descriptor_pb2.FieldDescriptorProto.LABEL_OPTIONAL
    f_control_type.type = descriptor_pb2.FieldDescriptorProto.TYPE_ENUM
    f_control_type.type_name = ".genie_msgs_pb.msg.RetargetInfo.ControlType"

    f_targets = retarget_info.field.add()
    f_targets.name = "target_joint_positions"
    f_targets.number = 3
    f_targets.label = descriptor_pb2.FieldDescriptorProto.LABEL_REPEATED
    f_targets.type = descriptor_pb2.FieldDescriptorProto.TYPE_DOUBLE

    pool = descriptor_pool.DescriptorPool()
    pool.Add(file_proto)

    msg_cls = get_message_class(pool, "genie_msgs_pb.msg.RetargetInfo")

    msg_cls.GROUP_LEFT_TOOL = _LEFT_GROUP_ID
    msg_cls.GROUP_RIGHT_TOOL = _RIGHT_GROUP_ID
    msg_cls.ABS_JOINT = _ABS_JOINT_CONTROL_TYPE

    return msg_cls


RetargetInfo = _build_messages()

__all__ = ["RetargetInfo"]
