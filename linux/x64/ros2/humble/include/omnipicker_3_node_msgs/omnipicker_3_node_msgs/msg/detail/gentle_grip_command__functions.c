// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from omnipicker_3_node_msgs:msg/GentleGripCommand.idl
// generated code does not contain a copyright notice
#include "omnipicker_3_node_msgs/msg/detail/gentle_grip_command__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


bool
omnipicker_3_node_msgs__msg__GentleGripCommand__init(omnipicker_3_node_msgs__msg__GentleGripCommand * msg)
{
  if (!msg) {
    return false;
  }
  // use_current_limit
  // current_limit_ma
  return true;
}

void
omnipicker_3_node_msgs__msg__GentleGripCommand__fini(omnipicker_3_node_msgs__msg__GentleGripCommand * msg)
{
  if (!msg) {
    return;
  }
  // use_current_limit
  // current_limit_ma
}

bool
omnipicker_3_node_msgs__msg__GentleGripCommand__are_equal(const omnipicker_3_node_msgs__msg__GentleGripCommand * lhs, const omnipicker_3_node_msgs__msg__GentleGripCommand * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // use_current_limit
  if (lhs->use_current_limit != rhs->use_current_limit) {
    return false;
  }
  // current_limit_ma
  if (lhs->current_limit_ma != rhs->current_limit_ma) {
    return false;
  }
  return true;
}

bool
omnipicker_3_node_msgs__msg__GentleGripCommand__copy(
  const omnipicker_3_node_msgs__msg__GentleGripCommand * input,
  omnipicker_3_node_msgs__msg__GentleGripCommand * output)
{
  if (!input || !output) {
    return false;
  }
  // use_current_limit
  output->use_current_limit = input->use_current_limit;
  // current_limit_ma
  output->current_limit_ma = input->current_limit_ma;
  return true;
}

omnipicker_3_node_msgs__msg__GentleGripCommand *
omnipicker_3_node_msgs__msg__GentleGripCommand__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  omnipicker_3_node_msgs__msg__GentleGripCommand * msg = (omnipicker_3_node_msgs__msg__GentleGripCommand *)allocator.allocate(sizeof(omnipicker_3_node_msgs__msg__GentleGripCommand), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(omnipicker_3_node_msgs__msg__GentleGripCommand));
  bool success = omnipicker_3_node_msgs__msg__GentleGripCommand__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
omnipicker_3_node_msgs__msg__GentleGripCommand__destroy(omnipicker_3_node_msgs__msg__GentleGripCommand * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    omnipicker_3_node_msgs__msg__GentleGripCommand__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
omnipicker_3_node_msgs__msg__GentleGripCommand__Sequence__init(omnipicker_3_node_msgs__msg__GentleGripCommand__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  omnipicker_3_node_msgs__msg__GentleGripCommand * data = NULL;

  if (size) {
    data = (omnipicker_3_node_msgs__msg__GentleGripCommand *)allocator.zero_allocate(size, sizeof(omnipicker_3_node_msgs__msg__GentleGripCommand), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = omnipicker_3_node_msgs__msg__GentleGripCommand__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        omnipicker_3_node_msgs__msg__GentleGripCommand__fini(&data[i - 1]);
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
omnipicker_3_node_msgs__msg__GentleGripCommand__Sequence__fini(omnipicker_3_node_msgs__msg__GentleGripCommand__Sequence * array)
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
      omnipicker_3_node_msgs__msg__GentleGripCommand__fini(&array->data[i]);
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

omnipicker_3_node_msgs__msg__GentleGripCommand__Sequence *
omnipicker_3_node_msgs__msg__GentleGripCommand__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  omnipicker_3_node_msgs__msg__GentleGripCommand__Sequence * array = (omnipicker_3_node_msgs__msg__GentleGripCommand__Sequence *)allocator.allocate(sizeof(omnipicker_3_node_msgs__msg__GentleGripCommand__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = omnipicker_3_node_msgs__msg__GentleGripCommand__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
omnipicker_3_node_msgs__msg__GentleGripCommand__Sequence__destroy(omnipicker_3_node_msgs__msg__GentleGripCommand__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    omnipicker_3_node_msgs__msg__GentleGripCommand__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
omnipicker_3_node_msgs__msg__GentleGripCommand__Sequence__are_equal(const omnipicker_3_node_msgs__msg__GentleGripCommand__Sequence * lhs, const omnipicker_3_node_msgs__msg__GentleGripCommand__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!omnipicker_3_node_msgs__msg__GentleGripCommand__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
omnipicker_3_node_msgs__msg__GentleGripCommand__Sequence__copy(
  const omnipicker_3_node_msgs__msg__GentleGripCommand__Sequence * input,
  omnipicker_3_node_msgs__msg__GentleGripCommand__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(omnipicker_3_node_msgs__msg__GentleGripCommand);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    omnipicker_3_node_msgs__msg__GentleGripCommand * data =
      (omnipicker_3_node_msgs__msg__GentleGripCommand *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!omnipicker_3_node_msgs__msg__GentleGripCommand__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          omnipicker_3_node_msgs__msg__GentleGripCommand__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!omnipicker_3_node_msgs__msg__GentleGripCommand__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
