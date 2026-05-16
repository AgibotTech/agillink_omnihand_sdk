# generated from rosidl_generator_py/resource/_idl.py.em
# with input from omnihand_2025_node_msgs:msg/TactileSensor.idl
# generated code does not contain a copyright notice


# Import statements for member types

# Member 'thumb'
# Member 'index'
# Member 'middle'
# Member 'ring'
# Member 'little'
# Member 'palm'
# Member 'dorsum'
import array  # noqa: E402, I100

import builtins  # noqa: E402, I100

import rosidl_parser.definition  # noqa: E402, I100


class Metaclass_TactileSensor(type):
    """Metaclass of message 'TactileSensor'."""

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
            module = import_type_support('omnihand_2025_node_msgs')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'omnihand_2025_node_msgs.msg.TactileSensor')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__msg__tactile_sensor
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__msg__tactile_sensor
            cls._CONVERT_TO_PY = module.convert_to_py_msg__msg__tactile_sensor
            cls._TYPE_SUPPORT = module.type_support_msg__msg__tactile_sensor
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__msg__tactile_sensor

            from std_msgs.msg import Header
            if Header.__class__._TYPE_SUPPORT is None:
                Header.__class__.__import_type_support__()

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class TactileSensor(metaclass=Metaclass_TactileSensor):
    """Message class 'TactileSensor'."""

    __slots__ = [
        '_header',
        '_thumb',
        '_index',
        '_middle',
        '_ring',
        '_little',
        '_palm',
        '_dorsum',
    ]

    _fields_and_field_types = {
        'header': 'std_msgs/Header',
        'thumb': 'sequence<uint8>',
        'index': 'sequence<uint8>',
        'middle': 'sequence<uint8>',
        'ring': 'sequence<uint8>',
        'little': 'sequence<uint8>',
        'palm': 'sequence<uint8>',
        'dorsum': 'sequence<uint8>',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.NamespacedType(['std_msgs', 'msg'], 'Header'),  # noqa: E501
        rosidl_parser.definition.UnboundedSequence(rosidl_parser.definition.BasicType('uint8')),  # noqa: E501
        rosidl_parser.definition.UnboundedSequence(rosidl_parser.definition.BasicType('uint8')),  # noqa: E501
        rosidl_parser.definition.UnboundedSequence(rosidl_parser.definition.BasicType('uint8')),  # noqa: E501
        rosidl_parser.definition.UnboundedSequence(rosidl_parser.definition.BasicType('uint8')),  # noqa: E501
        rosidl_parser.definition.UnboundedSequence(rosidl_parser.definition.BasicType('uint8')),  # noqa: E501
        rosidl_parser.definition.UnboundedSequence(rosidl_parser.definition.BasicType('uint8')),  # noqa: E501
        rosidl_parser.definition.UnboundedSequence(rosidl_parser.definition.BasicType('uint8')),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        from std_msgs.msg import Header
        self.header = kwargs.get('header', Header())
        self.thumb = array.array('B', kwargs.get('thumb', []))
        self.index = array.array('B', kwargs.get('index', []))
        self.middle = array.array('B', kwargs.get('middle', []))
        self.ring = array.array('B', kwargs.get('ring', []))
        self.little = array.array('B', kwargs.get('little', []))
        self.palm = array.array('B', kwargs.get('palm', []))
        self.dorsum = array.array('B', kwargs.get('dorsum', []))

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
        if self.header != other.header:
            return False
        if self.thumb != other.thumb:
            return False
        if self.index != other.index:
            return False
        if self.middle != other.middle:
            return False
        if self.ring != other.ring:
            return False
        if self.little != other.little:
            return False
        if self.palm != other.palm:
            return False
        if self.dorsum != other.dorsum:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @builtins.property
    def header(self):
        """Message field 'header'."""
        return self._header

    @header.setter
    def header(self, value):
        if __debug__:
            from std_msgs.msg import Header
            assert \
                isinstance(value, Header), \
                "The 'header' field must be a sub message of type 'Header'"
        self._header = value

    @builtins.property
    def thumb(self):
        """Message field 'thumb'."""
        return self._thumb

    @thumb.setter
    def thumb(self, value):
        if isinstance(value, array.array):
            assert value.typecode == 'B', \
                "The 'thumb' array.array() must have the type code of 'B'"
            self._thumb = value
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
                "The 'thumb' field must be a set or sequence and each value of type 'int' and each unsigned integer in [0, 255]"
        self._thumb = array.array('B', value)

    @builtins.property
    def index(self):
        """Message field 'index'."""
        return self._index

    @index.setter
    def index(self, value):
        if isinstance(value, array.array):
            assert value.typecode == 'B', \
                "The 'index' array.array() must have the type code of 'B'"
            self._index = value
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
                "The 'index' field must be a set or sequence and each value of type 'int' and each unsigned integer in [0, 255]"
        self._index = array.array('B', value)

    @builtins.property
    def middle(self):
        """Message field 'middle'."""
        return self._middle

    @middle.setter
    def middle(self, value):
        if isinstance(value, array.array):
            assert value.typecode == 'B', \
                "The 'middle' array.array() must have the type code of 'B'"
            self._middle = value
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
                "The 'middle' field must be a set or sequence and each value of type 'int' and each unsigned integer in [0, 255]"
        self._middle = array.array('B', value)

    @builtins.property
    def ring(self):
        """Message field 'ring'."""
        return self._ring

    @ring.setter
    def ring(self, value):
        if isinstance(value, array.array):
            assert value.typecode == 'B', \
                "The 'ring' array.array() must have the type code of 'B'"
            self._ring = value
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
                "The 'ring' field must be a set or sequence and each value of type 'int' and each unsigned integer in [0, 255]"
        self._ring = array.array('B', value)

    @builtins.property
    def little(self):
        """Message field 'little'."""
        return self._little

    @little.setter
    def little(self, value):
        if isinstance(value, array.array):
            assert value.typecode == 'B', \
                "The 'little' array.array() must have the type code of 'B'"
            self._little = value
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
                "The 'little' field must be a set or sequence and each value of type 'int' and each unsigned integer in [0, 255]"
        self._little = array.array('B', value)

    @builtins.property
    def palm(self):
        """Message field 'palm'."""
        return self._palm

    @palm.setter
    def palm(self, value):
        if isinstance(value, array.array):
            assert value.typecode == 'B', \
                "The 'palm' array.array() must have the type code of 'B'"
            self._palm = value
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
                "The 'palm' field must be a set or sequence and each value of type 'int' and each unsigned integer in [0, 255]"
        self._palm = array.array('B', value)

    @builtins.property
    def dorsum(self):
        """Message field 'dorsum'."""
        return self._dorsum

    @dorsum.setter
    def dorsum(self, value):
        if isinstance(value, array.array):
            assert value.typecode == 'B', \
                "The 'dorsum' array.array() must have the type code of 'B'"
            self._dorsum = value
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
                "The 'dorsum' field must be a set or sequence and each value of type 'int' and each unsigned integer in [0, 255]"
        self._dorsum = array.array('B', value)
