// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from omnipicker_3_node_msgs:msg/GentleGripState.idl
// generated code does not contain a copyright notice
#include "omnipicker_3_node_msgs/msg/detail/gentle_grip_state__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/detail/header__functions.h"
// Member `raw_response`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

bool
omnipicker_3_node_msgs__msg__GentleGripState__init(omnipicker_3_node_msgs__msg__GentleGripState * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__init(&msg->header)) {
    omnipicker_3_node_msgs__msg__GentleGripState__fini(msg);
    return false;
  }
  // raw_response
  if (!rosidl_runtime_c__uint8__Sequence__init(&msg->raw_response, 0)) {
    omnipicker_3_node_msgs__msg__GentleGripState__fini(msg);
    return false;
  }
  return true;
}

void
omnipicker_3_node_msgs__msg__GentleGripState__fini(omnipicker_3_node_msgs__msg__GentleGripState * msg)
{
  if (!msg) {
    return;
  }
  // header
  std_msgs__msg__Header__fini(&msg->header);
  // raw_response
  rosidl_runtime_c__uint8__Sequence__fini(&msg->raw_response);
}

bool
omnipicker_3_node_msgs__msg__GentleGripState__are_equal(const omnipicker_3_node_msgs__msg__GentleGripState * lhs, const omnipicker_3_node_msgs__msg__GentleGripState * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__are_equal(
      &(lhs->header), &(rhs->header)))
  {
    return false;
  }
  // raw_response
  if (!rosidl_runtime_c__uint8__Sequence__are_equal(
      &(lhs->raw_response), &(rhs->raw_response)))
  {
    return false;
  }
  return true;
}

bool
omnipicker_3_node_msgs__msg__GentleGripState__copy(
  const omnipicker_3_node_msgs__msg__GentleGripState * input,
  omnipicker_3_node_msgs__msg__GentleGripState * output)
{
  if (!input || !output) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__copy(
      &(input->header), &(output->header)))
  {
    return false;
  }
  // raw_response
  if (!rosidl_runtime_c__uint8__Sequence__copy(
      &(input->raw_response), &(output->raw_response)))
  {
    return false;
  }
  return true;
}

omnipicker_3_node_msgs__msg__GentleGripState *
omnipicker_3_node_msgs__msg__GentleGripState__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  omnipicker_3_node_msgs__msg__GentleGripState * msg = (omnipicker_3_node_msgs__msg__GentleGripState *)allocator.allocate(sizeof(omnipicker_3_node_msgs__msg__GentleGripState), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(omnipicker_3_node_msgs__msg__GentleGripState));
  bool success = omnipicker_3_node_msgs__msg__GentleGripState__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
omnipicker_3_node_msgs__msg__GentleGripState__destroy(omnipicker_3_node_msgs__msg__GentleGripState * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    omnipicker_3_node_msgs__msg__GentleGripState__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
omnipicker_3_node_msgs__msg__GentleGripState__Sequence__init(omnipicker_3_node_msgs__msg__GentleGripState__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  omnipicker_3_node_msgs__msg__GentleGripState * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(omnipicker_3_node_msgs__msg__GentleGripState)) {
      return false;
    }
    data = (omnipicker_3_node_msgs__msg__GentleGripState *)allocator.zero_allocate(size, sizeof(omnipicker_3_node_msgs__msg__GentleGripState), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = omnipicker_3_node_msgs__msg__GentleGripState__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        omnipicker_3_node_msgs__msg__GentleGripState__fini(&data[i - 1]);
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
omnipicker_3_node_msgs__msg__GentleGripState__Sequence__fini(omnipicker_3_node_msgs__msg__GentleGripState__Sequence * array)
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
      omnipicker_3_node_msgs__msg__GentleGripState__fini(&array->data[i]);
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

omnipicker_3_node_msgs__msg__GentleGripState__Sequence *
omnipicker_3_node_msgs__msg__GentleGripState__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  omnipicker_3_node_msgs__msg__GentleGripState__Sequence * array = (omnipicker_3_node_msgs__msg__GentleGripState__Sequence *)allocator.allocate(sizeof(omnipicker_3_node_msgs__msg__GentleGripState__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = omnipicker_3_node_msgs__msg__GentleGripState__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
omnipicker_3_node_msgs__msg__GentleGripState__Sequence__destroy(omnipicker_3_node_msgs__msg__GentleGripState__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    omnipicker_3_node_msgs__msg__GentleGripState__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
omnipicker_3_node_msgs__msg__GentleGripState__Sequence__are_equal(const omnipicker_3_node_msgs__msg__GentleGripState__Sequence * lhs, const omnipicker_3_node_msgs__msg__GentleGripState__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!omnipicker_3_node_msgs__msg__GentleGripState__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
omnipicker_3_node_msgs__msg__GentleGripState__Sequence__copy(
  const omnipicker_3_node_msgs__msg__GentleGripState__Sequence * input,
  omnipicker_3_node_msgs__msg__GentleGripState__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(omnipicker_3_node_msgs__msg__GentleGripState)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(omnipicker_3_node_msgs__msg__GentleGripState);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    omnipicker_3_node_msgs__msg__GentleGripState * data =
      (omnipicker_3_node_msgs__msg__GentleGripState *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!omnipicker_3_node_msgs__msg__GentleGripState__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          omnipicker_3_node_msgs__msg__GentleGripState__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!omnipicker_3_node_msgs__msg__GentleGripState__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
