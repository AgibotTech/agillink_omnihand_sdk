"""Helpers for dynamic protobuf message classes (protobuf 3.x vs 4+)."""

from __future__ import annotations

from google.protobuf import descriptor_pool, message_factory


def get_message_class(pool: descriptor_pool.DescriptorPool, full_name: str):
    desc = pool.FindMessageTypeByName(full_name)
    gm = getattr(message_factory, "GetMessageClass", None)
    if gm is not None:
        return gm(desc)
    return message_factory.MessageFactory(pool).GetPrototype(desc)
