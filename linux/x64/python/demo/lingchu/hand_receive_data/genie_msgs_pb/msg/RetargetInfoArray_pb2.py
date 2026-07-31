"""
Minimal protobuf definitions for `RetargetInfoArray`.

Compatible with `hand_send_data` UDP protobuf format (protobuf 6+ API).
"""

from __future__ import annotations

from google.protobuf import descriptor_pb2, descriptor_pool

from ..dynamic_util import get_message_class

from .RetargetInfo_pb2 import (
    _ABS_JOINT_CONTROL_TYPE as _ABS_JOINT_CONTROL_TYPE,  # type: ignore
    _LEFT_GROUP_ID as _LEFT_GROUP_ID,  # type: ignore
    _RIGHT_GROUP_ID as _RIGHT_GROUP_ID,  # type: ignore
)


def _build_messages():
    file_proto = descriptor_pb2.FileDescriptorProto()
    file_proto.name = "genie_msgs_pb/msg/RetargetInfoArray.proto"
    file_proto.package = "genie_msgs_pb.msg"
    file_proto.syntax = "proto3"

    retarget_info = file_proto.message_type.add()
    retarget_info.name = "RetargetInfo"

    group_id = retarget_info.enum_type.add()
    group_id.name = "GroupId"
    group_id.value.add(name="GROUP_UNSPECIFIED", number=0)
    group_id.value.add(name="GROUP_LEFT_TOOL", number=_LEFT_GROUP_ID)
    group_id.value.add(name="GROUP_RIGHT_TOOL", number=_RIGHT_GROUP_ID)

    control_type = retarget_info.enum_type.add()
    control_type.name = "ControlType"
    control_type.value.add(name="CONTROL_UNSPECIFIED", number=0)
    control_type.value.add(name="ABS_JOINT", number=_ABS_JOINT_CONTROL_TYPE)

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

    retarget_info_array = file_proto.message_type.add()
    retarget_info_array.name = "RetargetInfoArray"

    f_infos = retarget_info_array.field.add()
    f_infos.name = "retarget_infos"
    f_infos.number = 1
    f_infos.label = descriptor_pb2.FieldDescriptorProto.LABEL_REPEATED
    f_infos.type = descriptor_pb2.FieldDescriptorProto.TYPE_MESSAGE
    f_infos.type_name = ".genie_msgs_pb.msg.RetargetInfo"

    pool = descriptor_pool.DescriptorPool()
    pool.Add(file_proto)

    array_cls = get_message_class(pool, "genie_msgs_pb.msg.RetargetInfoArray")
    return array_cls


RetargetInfoArray = _build_messages()

__all__ = ["RetargetInfoArray"]
