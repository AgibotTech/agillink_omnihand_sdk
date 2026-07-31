# generated from rosidl_generator_py/resource/_idl.py.em
# with input from omnihand_pro_2025_node_msgs:msg/TactileSensorData.idl
# generated code does not contain a copyright notice


# Import statements for member types

# Member 'channel_value'
# Member 'capa_approach'
import array  # noqa: E402, I100

import builtins  # noqa: E402, I100

import rosidl_parser.definition  # noqa: E402, I100


class Metaclass_TactileSensorData(type):
    """Metaclass of message 'TactileSensorData'."""

    _CREATE_ROS_MESSAGE = None
    _CONVERT_FROM_PY = None
    _CONVERT_TO_PY = None
    _DESTROY_ROS_MESSAGE = None
    _TYPE_SUPPORT = None

    __constants = {
    }

    @classmethod
    def __import_type_support__(cls):
        try:
            from rosidl_generator_py import import_type_support
            module = import_type_support('omnihand_pro_2025_node_msgs')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'omnihand_pro_2025_node_msgs.msg.TactileSensorData')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__msg__tactile_sensor_data
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__msg__tactile_sensor_data
            cls._CONVERT_TO_PY = module.convert_to_py_msg__msg__tactile_sensor_data
            cls._TYPE_SUPPORT = module.type_support_msg__msg__tactile_sensor_data
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__msg__tactile_sensor_data

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class TactileSensorData(metaclass=Metaclass_TactileSensorData):
    """Message class 'TactileSensorData'."""

    __slots__ = [
        '_online_state',
        '_channel_value',
        '_normal_force',
        '_tangent_force',
        '_tangent_force_angle',
        '_capa_approach',
    ]

    _fields_and_field_types = {
        'online_state': 'uint8',
        'channel_value': 'sequence<uint32>',
        'normal_force': 'uint16',
        'tangent_force': 'uint16',
        'tangent_force_angle': 'uint16',
        'capa_approach': 'sequence<uint8>',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.BasicType('uint8'),  # noqa: E501
        rosidl_parser.definition.UnboundedSequence(rosidl_parser.definition.BasicType('uint32')),  # noqa: E501
        rosidl_parser.definition.BasicType('uint16'),  # noqa: E501
        rosidl_parser.definition.BasicType('uint16'),  # noqa: E501
        rosidl_parser.definition.BasicType('uint16'),  # noqa: E501
        rosidl_parser.definition.UnboundedSequence(rosidl_parser.definition.BasicType('uint8')),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        self.online_state = kwargs.get('online_state', int())
        self.channel_value = array.array('I', kwargs.get('channel_value', []))
        self.normal_force = kwargs.get('normal_force', int())
        self.tangent_force = kwargs.get('tangent_force', int())
        self.tangent_force_angle = kwargs.get('tangent_force_angle', int())
        self.capa_approach = array.array('B', kwargs.get('capa_approach', []))

    def __repr__(self):
        typename = self.__class__.__module__.split('.')
        typename.pop()
        typename.append(self.__class__.__name__)
        args = []
        for s, t in zip(self.__slots__, self.SLOT_TYPES):
            field = getattr(self, s)
            fieldstr = repr(field)
            # We use Python array type for fields that can be directly stored
            # in them, and "normal" sequences for everything else.  If it is
            # a type that we store in an array, strip off the 'array' portion.
            if (
                isinstance(t, rosidl_parser.definition.AbstractSequence) and
                isinstance(t.value_type, rosidl_parser.definition.BasicType) and
                t.value_type.typename in ['float', 'double', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'int64', 'uint64']
            ):
                if len(field) == 0:
                    fieldstr = '[]'
                else:
                    assert fieldstr.startswith('array(')
                    prefix = "array('X', "
                    suffix = ')'
                    fieldstr = fieldstr[len(prefix):-len(suffix)]
            args.append(s[1:] + '=' + fieldstr)
        return '%s(%s)' % ('.'.join(typename), ', '.join(args))

    def __eq__(self, other):
        if not isinstance(other, self.__class__):
            return False
        if self.online_state != other.online_state:
            return False
        if self.channel_value != other.channel_value:
            return False
        if self.normal_force != other.normal_force:
            return False
        if self.tangent_force != other.tangent_force:
            return False
        if self.tangent_force_angle != other.tangent_force_angle:
            return False
        if self.capa_approach != other.capa_approach:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @builtins.property
    def online_state(self):
        """Message field 'online_state'."""
        return self._online_state

    @online_state.setter
    def online_state(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'online_state' field must be of type 'int'"
            assert value >= 0 and value < 256, \
                "The 'online_state' field must be an unsigned integer in [0, 255]"
        self._online_state = value

    @builtins.property
    def channel_value(self):
        """Message field 'channel_value'."""
        return self._channel_value

    @channel_value.setter
    def channel_value(self, value):
        if isinstance(value, array.array):
            assert value.typecode == 'I', \
                "The 'channel_value' array.array() must have the type code of 'I'"
            self._channel_value = value
            return
        if __debug__:
            from collections.abc import Sequence
            from collections.abc import Set
            from collections import UserList
            from collections import UserString
            assert \
                ((isinstance(value, Sequence) or
                  isinstance(value, Set) or
                  isinstance(value, UserList)) and
                 not isinstance(value, str) and
                 not isinstance(value, UserString) and
                 all(isinstance(v, int) for v in value) and
                 all(val >= 0 and val < 4294967296 for val in value)), \
                "The 'channel_value' field must be a set or sequence and each value of type 'int' and each unsigned integer in [0, 4294967295]"
        self._channel_value = array.array('I', value)

    @builtins.property
    def normal_force(self):
        """Message field 'normal_force'."""
        return self._normal_force

    @normal_force.setter
    def normal_force(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'normal_force' field must be of type 'int'"
            assert value >= 0 and value < 65536, \
                "The 'normal_force' field must be an unsigned integer in [0, 65535]"
        self._normal_force = value

    @builtins.property
    def tangent_force(self):
        """Message field 'tangent_force'."""
        return self._tangent_force

    @tangent_force.setter
    def tangent_force(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'tangent_force' field must be of type 'int'"
            assert value >= 0 and value < 65536, \
                "The 'tangent_force' field must be an unsigned integer in [0, 65535]"
        self._tangent_force = value

    @builtins.property
    def tangent_force_angle(self):
        """Message field 'tangent_force_angle'."""
        return self._tangent_force_angle

    @tangent_force_angle.setter
    def tangent_force_angle(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'tangent_force_angle' field must be of type 'int'"
            assert value >= 0 and value < 65536, \
                "The 'tangent_force_angle' field must be an unsigned integer in [0, 65535]"
        self._tangent_force_angle = value

    @builtins.property
    def capa_approach(self):
        """Message field 'capa_approach'."""
        return self._capa_approach

    @capa_approach.setter
    def capa_approach(self, value):
        if isinstance(value, array.array):
            assert value.typecode == 'B', \
                "The 'capa_approach' array.array() must have the type code of 'B'"
            self._capa_approach = value
            return
        if __debug__:
            from collections.abc import Sequence
            from collections.abc import Set
            from collections import UserList
            from collections import UserString
            assert \
                ((isinstance(value, Sequence) or
                  isinstance(value, Set) or
                  isinstance(value, UserList)) and
                 not isinstance(value, str) and
                 not isinstance(value, UserString) and
                 all(isinstance(v, int) for v in value) and
                 all(val >= 0 and val < 256 for val in value)), \
                "The 'capa_approach' field must be a set or sequence and each value of type 'int' and each unsigned integer in [0, 255]"
        self._capa_approach = array.array('B', value)
