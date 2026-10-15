// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from omnipicker_3_node_msgs:msg/StrokeCommand.idl
// generated code does not contain a copyright notice
#include "omnipicker_3_node_msgs/msg/detail/stroke_command__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


bool
omnipicker_3_node_msgs__msg__StrokeCommand__init(omnipicker_3_node_msgs__msg__StrokeCommand * msg)
{
  if (!msg) {
    return false;
  }
  // stroke_mm
  // use_speed
  // speed_rpm
  // use_current
  // current_ma
  return true;
}

void
omnipicker_3_node_msgs__msg__StrokeCommand__fini(omnipicker_3_node_msgs__msg__StrokeCommand * msg)
{
  if (!msg) {
    return;
  }
  // stroke_mm
  // use_speed
  // speed_rpm
  // use_current
  // current_ma
}

bool
omnipicker_3_node_msgs__msg__StrokeCommand__are_equal(const omnipicker_3_node_msgs__msg__StrokeCommand * lhs, const omnipicker_3_node_msgs__msg__StrokeCommand * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // stroke_mm
  if (lhs->stroke_mm != rhs->stroke_mm) {
    return false;
  }
  // use_speed
  if (lhs->use_speed != rhs->use_speed) {
    return false;
  }
  // speed_rpm
  if (lhs->speed_rpm != rhs->speed_rpm) {
    return false;
  }
  // use_current
  if (lhs->use_current != rhs->use_current) {
    return false;
  }
  // current_ma
  if (lhs->current_ma != rhs->current_ma) {
    return false;
  }
  return true;
}

bool
omnipicker_3_node_msgs__msg__StrokeCommand__copy(
  const omnipicker_3_node_msgs__msg__StrokeCommand * input,
  omnipicker_3_node_msgs__msg__StrokeCommand * output)
{
  if (!input || !output) {
    return false;
  }
  // stroke_mm
  output->stroke_mm = input->stroke_mm;
  // use_speed
  output->use_speed = input->use_speed;
  // speed_rpm
  output->speed_rpm = input->speed_rpm;
  // use_current
  output->use_current = input->use_current;
  // current_ma
  output->current_ma = input->current_ma;
  return true;
}

omnipicker_3_node_msgs__msg__StrokeCommand *
omnipicker_3_node_msgs__msg__StrokeCommand__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  omnipicker_3_node_msgs__msg__StrokeCommand * msg = (omnipicker_3_node_msgs__msg__StrokeCommand *)allocator.allocate(sizeof(omnipicker_3_node_msgs__msg__StrokeCommand), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(omnipicker_3_node_msgs__msg__StrokeCommand));
  bool success = omnipicker_3_node_msgs__msg__StrokeCommand__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
omnipicker_3_node_msgs__msg__StrokeCommand__destroy(omnipicker_3_node_msgs__msg__StrokeCommand * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    omnipicker_3_node_msgs__msg__StrokeCommand__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
omnipicker_3_node_msgs__msg__StrokeCommand__Sequence__init(omnipicker_3_node_msgs__msg__StrokeCommand__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  omnipicker_3_node_msgs__msg__StrokeCommand * data = NULL;

  if (size) {
    data = (omnipicker_3_node_msgs__msg__StrokeCommand *)allocator.zero_allocate(size, sizeof(omnipicker_3_node_msgs__msg__StrokeCommand), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = omnipicker_3_node_msgs__msg__StrokeCommand__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        omnipicker_3_node_msgs__msg__StrokeCommand__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
omnipicker_3_node_msgs__msg__StrokeCommand__Sequence__fini(omnipicker_3_node_msgs__msg__StrokeCommand__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      omnipicker_3_node_msgs__msg__StrokeCommand__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

omnipicker_3_node_msgs__msg__StrokeCommand__Sequence *
omnipicker_3_node_msgs__msg__StrokeCommand__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  omnipicker_3_node_msgs__msg__StrokeCommand__Sequence * array = (omnipicker_3_node_msgs__msg__StrokeCommand__Sequence *)allocator.allocate(sizeof(omnipicker_3_node_msgs__msg__StrokeCommand__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = omnipicker_3_node_msgs__msg__StrokeCommand__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
omnipicker_3_node_msgs__msg__StrokeCommand__Sequence__destroy(omnipicker_3_node_msgs__msg__StrokeCommand__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    omnipicker_3_node_msgs__msg__StrokeCommand__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
omnipicker_3_node_msgs__msg__StrokeCommand__Sequence__are_equal(const omnipicker_3_node_msgs__msg__StrokeCommand__Sequence * lhs, const omnipicker_3_node_msgs__msg__StrokeCommand__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!omnipicker_3_node_msgs__msg__StrokeCommand__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
omnipicker_3_node_msgs__msg__StrokeCommand__Sequence__copy(
  const omnipicker_3_node_msgs__msg__StrokeCommand__Sequence * input,
  omnipicker_3_node_msgs__msg__StrokeCommand__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(omnipicker_3_node_msgs__msg__StrokeCommand);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    omnipicker_3_node_msgs__msg__StrokeCommand * data =
      (omnipicker_3_node_msgs__msg__StrokeCommand *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!omnipicker_3_node_msgs__msg__StrokeCommand__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          omnipicker_3_node_msgs__msg__StrokeCommand__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!omnipicker_3_node_msgs__msg__StrokeCommand__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
