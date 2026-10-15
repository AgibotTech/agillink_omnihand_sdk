# generated from rosidl_generator_py/resource/_idl.py.em
# with input from omnipicker_3_node_msgs:msg/StrokeCommand.idl
# generated code does not contain a copyright notice


# Import statements for member types

import builtins  # noqa: E402, I100

import math  # noqa: E402, I100

import rosidl_parser.definition  # noqa: E402, I100


class Metaclass_StrokeCommand(type):
    """Metaclass of message 'StrokeCommand'."""

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
            module = import_type_support('omnipicker_3_node_msgs')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'omnipicker_3_node_msgs.msg.StrokeCommand')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__msg__stroke_command
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__msg__stroke_command
            cls._CONVERT_TO_PY = module.convert_to_py_msg__msg__stroke_command
            cls._TYPE_SUPPORT = module.type_support_msg__msg__stroke_command
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__msg__stroke_command

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class StrokeCommand(metaclass=Metaclass_StrokeCommand):
    """Message class 'StrokeCommand'."""

    __slots__ = [
        '_stroke_mm',
        '_use_speed',
        '_speed_rpm',
        '_use_current',
        '_current_ma',
    ]

    _fields_and_field_types = {
        'stroke_mm': 'double',
        'use_speed': 'boolean',
        'speed_rpm': 'int16',
        'use_current': 'boolean',
        'current_ma': 'int16',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.BasicType('double'),  # noqa: E501
        rosidl_parser.definition.BasicType('boolean'),  # noqa: E501
        rosidl_parser.definition.BasicType('int16'),  # noqa: E501
        rosidl_parser.definition.BasicType('boolean'),  # noqa: E501
        rosidl_parser.definition.BasicType('int16'),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        self.stroke_mm = kwargs.get('stroke_mm', float())
        self.use_speed = kwargs.get('use_speed', bool())
        self.speed_rpm = kwargs.get('speed_rpm', int())
        self.use_current = kwargs.get('use_current', bool())
        self.current_ma = kwargs.get('current_ma', int())

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
        if self.stroke_mm != other.stroke_mm:
            return False
        if self.use_speed != other.use_speed:
            return False
        if self.speed_rpm != other.speed_rpm:
            return False
        if self.use_current != other.use_current:
            return False
        if self.current_ma != other.current_ma:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @builtins.property
    def stroke_mm(self):
        """Message field 'stroke_mm'."""
        return self._stroke_mm

    @stroke_mm.setter
    def stroke_mm(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'stroke_mm' field must be of type 'float'"
            assert not (value < -1.7976931348623157e+308 or value > 1.7976931348623157e+308) or math.isinf(value), \
                "The 'stroke_mm' field must be a double in [-1.7976931348623157e+308, 1.7976931348623157e+308]"
        self._stroke_mm = value

    @builtins.property
    def use_speed(self):
        """Message field 'use_speed'."""
        return self._use_speed

    @use_speed.setter
    def use_speed(self, value):
        if __debug__:
            assert \
                isinstance(value, bool), \
                "The 'use_speed' field must be of type 'bool'"
        self._use_speed = value

    @builtins.property
    def speed_rpm(self):
        """Message field 'speed_rpm'."""
        return self._speed_rpm

    @speed_rpm.setter
    def speed_rpm(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'speed_rpm' field must be of type 'int'"
            assert value >= -32768 and value < 32768, \
                "The 'speed_rpm' field must be an integer in [-32768, 32767]"
        self._speed_rpm = value

    @builtins.property
    def use_current(self):
        """Message field 'use_current'."""
        return self._use_current

    @use_current.setter
    def use_current(self, value):
        if __debug__:
            assert \
                isinstance(value, bool), \
                "The 'use_current' field must be of type 'bool'"
        self._use_current = value

    @builtins.property
    def current_ma(self):
        """Message field 'current_ma'."""
        return self._current_ma

    @current_ma.setter
    def current_ma(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'current_ma' field must be of type 'int'"
            assert value >= -32768 and value < 32768, \
                "The 'current_ma' field must be an integer in [-32768, 32767]"
        self._current_ma = value
