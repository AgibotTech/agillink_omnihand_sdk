// generated from rosidl_generator_py/resource/_idl_support.c.em
// with input from omnipicker_3_node_msgs:msg/StrokeCommand.idl
// generated code does not contain a copyright notice
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <Python.h>
#include <stdbool.h>
#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-function"
#endif
#include "numpy/ndarrayobject.h"
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif
#include "rosidl_runtime_c/visibility_control.h"
#include "omnipicker_3_node_msgs/msg/detail/stroke_command__struct.h"
#include "omnipicker_3_node_msgs/msg/detail/stroke_command__functions.h"


ROSIDL_GENERATOR_C_EXPORT
bool omnipicker_3_node_msgs__msg__stroke_command__convert_from_py(PyObject * _pymsg, void * _ros_message)
{
  // check that the passed message is of the expected Python class
  {
    char full_classname_dest[57];
    {
      char * class_name = NULL;
      char * module_name = NULL;
      {
        PyObject * class_attr = PyObject_GetAttrString(_pymsg, "__class__");
        if (class_attr) {
          PyObject * name_attr = PyObject_GetAttrString(class_attr, "__name__");
          if (name_attr) {
            class_name = (char *)PyUnicode_1BYTE_DATA(name_attr);
            Py_DECREF(name_attr);
          }
          PyObject * module_attr = PyObject_GetAttrString(class_attr, "__module__");
          if (module_attr) {
            module_name = (char *)PyUnicode_1BYTE_DATA(module_attr);
            Py_DECREF(module_attr);
          }
          Py_DECREF(class_attr);
        }
      }
      if (!class_name || !module_name) {
        return false;
      }
      snprintf(full_classname_dest, sizeof(full_classname_dest), "%s.%s", module_name, class_name);
    }
    assert(strncmp("omnipicker_3_node_msgs.msg._stroke_command.StrokeCommand", full_classname_dest, 56) == 0);
  }
  omnipicker_3_node_msgs__msg__StrokeCommand * ros_message = _ros_message;
  {  // stroke_mm
    PyObject * field = PyObject_GetAttrString(_pymsg, "stroke_mm");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->stroke_mm = PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // use_speed
    PyObject * field = PyObject_GetAttrString(_pymsg, "use_speed");
    if (!field) {
      return false;
    }
    assert(PyBool_Check(field));
    ros_message->use_speed = (Py_True == field);
    Py_DECREF(field);
  }
  {  // speed_rpm
    PyObject * field = PyObject_GetAttrString(_pymsg, "speed_rpm");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->speed_rpm = (int16_t)PyLong_AsLong(field);
    Py_DECREF(field);
  }
  {  // use_current
    PyObject * field = PyObject_GetAttrString(_pymsg, "use_current");
    if (!field) {
      return false;
    }
    assert(PyBool_Check(field));
    ros_message->use_current = (Py_True == field);
    Py_DECREF(field);
  }
  {  // current_ma
    PyObject * field = PyObject_GetAttrString(_pymsg, "current_ma");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->current_ma = (int16_t)PyLong_AsLong(field);
    Py_DECREF(field);
  }

  return true;
}

ROSIDL_GENERATOR_C_EXPORT
PyObject * omnipicker_3_node_msgs__msg__stroke_command__convert_to_py(void * raw_ros_message)
{
  /* NOTE(esteve): Call constructor of StrokeCommand */
  PyObject * _pymessage = NULL;
  {
    PyObject * pymessage_module = PyImport_ImportModule("omnipicker_3_node_msgs.msg._stroke_command");
    assert(pymessage_module);
    PyObject * pymessage_class = PyObject_GetAttrString(pymessage_module, "StrokeCommand");
    assert(pymessage_class);
    Py_DECREF(pymessage_module);
    _pymessage = PyObject_CallObject(pymessage_class, NULL);
    Py_DECREF(pymessage_class);
    if (!_pymessage) {
      return NULL;
    }
  }
  omnipicker_3_node_msgs__msg__StrokeCommand * ros_message = (omnipicker_3_node_msgs__msg__StrokeCommand *)raw_ros_message;
  {  // stroke_mm
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->stroke_mm);
    {
      int rc = PyObject_SetAttrString(_pymessage, "stroke_mm", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // use_speed
    PyObject * field = NULL;
    field = PyBool_FromLong(ros_message->use_speed ? 1 : 0);
    {
      int rc = PyObject_SetAttrString(_pymessage, "use_speed", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // speed_rpm
    PyObject * field = NULL;
    field = PyLong_FromLong(ros_message->speed_rpm);
    {
      int rc = PyObject_SetAttrString(_pymessage, "speed_rpm", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // use_current
    PyObject * field = NULL;
    field = PyBool_FromLong(ros_message->use_current ? 1 : 0);
    {
      int rc = PyObject_SetAttrString(_pymessage, "use_current", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // current_ma
    PyObject * field = NULL;
    field = PyLong_FromLong(ros_message->current_ma);
    {
      int rc = PyObject_SetAttrString(_pymessage, "current_ma", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }

  // ownership of _pymessage is transferred to the caller
  return _pymessage;
}
