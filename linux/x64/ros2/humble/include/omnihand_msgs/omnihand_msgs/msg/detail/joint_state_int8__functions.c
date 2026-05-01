// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from omnihand_msgs:msg/JointStateInt8.idl
// generated code does not contain a copyright notice
#include "omnihand_msgs/msg/detail/joint_state_int8__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/detail/header__functions.h"
// Member `name`
#include "rosidl_runtime_c/string_functions.h"
// Member `data`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

bool
omnihand_msgs__msg__JointStateInt8__init(omnihand_msgs__msg__JointStateInt8 * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__init(&msg->header)) {
    omnihand_msgs__msg__JointStateInt8__fini(msg);
    return false;
  }
  // name
  if (!rosidl_runtime_c__String__Sequence__init(&msg->name, 0)) {
    omnihand_msgs__msg__JointStateInt8__fini(msg);
    return false;
  }
  // data
  if (!rosidl_runtime_c__int8__Sequence__init(&msg->data, 0)) {
    omnihand_msgs__msg__JointStateInt8__fini(msg);
    return false;
  }
  return true;
}

void
omnihand_msgs__msg__JointStateInt8__fini(omnihand_msgs__msg__JointStateInt8 * msg)
{
  if (!msg) {
    return;
  }
  // header
  std_msgs__msg__Header__fini(&msg->header);
  // name
  rosidl_runtime_c__String__Sequence__fini(&msg->name);
  // data
  rosidl_runtime_c__int8__Sequence__fini(&msg->data);
}

bool
omnihand_msgs__msg__JointStateInt8__are_equal(const omnihand_msgs__msg__JointStateInt8 * lhs, const omnihand_msgs__msg__JointStateInt8 * rhs)
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
  // name
  if (!rosidl_runtime_c__String__Sequence__are_equal(
      &(lhs->name), &(rhs->name)))
  {
    return false;
  }
  // data
  if (!rosidl_runtime_c__int8__Sequence__are_equal(
      &(lhs->data), &(rhs->data)))
  {
    return false;
  }
  return true;
}

bool
omnihand_msgs__msg__JointStateInt8__copy(
  const omnihand_msgs__msg__JointStateInt8 * input,
  omnihand_msgs__msg__JointStateInt8 * output)
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
  // name
  if (!rosidl_runtime_c__String__Sequence__copy(
      &(input->name), &(output->name)))
  {
    return false;
  }
  // data
  if (!rosidl_runtime_c__int8__Sequence__copy(
      &(input->data), &(output->data)))
  {
    return false;
  }
  return true;
}

omnihand_msgs__msg__JointStateInt8 *
omnihand_msgs__msg__JointStateInt8__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  omnihand_msgs__msg__JointStateInt8 * msg = (omnihand_msgs__msg__JointStateInt8 *)allocator.allocate(sizeof(omnihand_msgs__msg__JointStateInt8), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(omnihand_msgs__msg__JointStateInt8));
  bool success = omnihand_msgs__msg__JointStateInt8__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
omnihand_msgs__msg__JointStateInt8__destroy(omnihand_msgs__msg__JointStateInt8 * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    omnihand_msgs__msg__JointStateInt8__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
omnihand_msgs__msg__JointStateInt8__Sequence__init(omnihand_msgs__msg__JointStateInt8__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  omnihand_msgs__msg__JointStateInt8 * data = NULL;

  if (size) {
    data = (omnihand_msgs__msg__JointStateInt8 *)allocator.zero_allocate(size, sizeof(omnihand_msgs__msg__JointStateInt8), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = omnihand_msgs__msg__JointStateInt8__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        omnihand_msgs__msg__JointStateInt8__fini(&data[i - 1]);
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
omnihand_msgs__msg__JointStateInt8__Sequence__fini(omnihand_msgs__msg__JointStateInt8__Sequence * array)
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
      omnihand_msgs__msg__JointStateInt8__fini(&array->data[i]);
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

omnihand_msgs__msg__JointStateInt8__Sequence *
omnihand_msgs__msg__JointStateInt8__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  omnihand_msgs__msg__JointStateInt8__Sequence * array = (omnihand_msgs__msg__JointStateInt8__Sequence *)allocator.allocate(sizeof(omnihand_msgs__msg__JointStateInt8__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = omnihand_msgs__msg__JointStateInt8__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
omnihand_msgs__msg__JointStateInt8__Sequence__destroy(omnihand_msgs__msg__JointStateInt8__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    omnihand_msgs__msg__JointStateInt8__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
omnihand_msgs__msg__JointStateInt8__Sequence__are_equal(const omnihand_msgs__msg__JointStateInt8__Sequence * lhs, const omnihand_msgs__msg__JointStateInt8__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!omnihand_msgs__msg__JointStateInt8__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
omnihand_msgs__msg__JointStateInt8__Sequence__copy(
  const omnihand_msgs__msg__JointStateInt8__Sequence * input,
  omnihand_msgs__msg__JointStateInt8__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(omnihand_msgs__msg__JointStateInt8);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    omnihand_msgs__msg__JointStateInt8 * data =
      (omnihand_msgs__msg__JointStateInt8 *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!omnihand_msgs__msg__JointStateInt8__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          omnihand_msgs__msg__JointStateInt8__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!omnihand_msgs__msg__JointStateInt8__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
