// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from omnihand_2025_node_msgs:msg/TactileSensor.idl
// generated code does not contain a copyright notice
#include "omnihand_2025_node_msgs/msg/detail/tactile_sensor__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/detail/header__functions.h"
// Member `thumb`
// Member `index`
// Member `middle`
// Member `ring`
// Member `little`
// Member `palm`
// Member `dorsum`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

bool
omnihand_2025_node_msgs__msg__TactileSensor__init(omnihand_2025_node_msgs__msg__TactileSensor * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__init(&msg->header)) {
    omnihand_2025_node_msgs__msg__TactileSensor__fini(msg);
    return false;
  }
  // thumb
  if (!rosidl_runtime_c__uint8__Sequence__init(&msg->thumb, 0)) {
    omnihand_2025_node_msgs__msg__TactileSensor__fini(msg);
    return false;
  }
  // index
  if (!rosidl_runtime_c__uint8__Sequence__init(&msg->index, 0)) {
    omnihand_2025_node_msgs__msg__TactileSensor__fini(msg);
    return false;
  }
  // middle
  if (!rosidl_runtime_c__uint8__Sequence__init(&msg->middle, 0)) {
    omnihand_2025_node_msgs__msg__TactileSensor__fini(msg);
    return false;
  }
  // ring
  if (!rosidl_runtime_c__uint8__Sequence__init(&msg->ring, 0)) {
    omnihand_2025_node_msgs__msg__TactileSensor__fini(msg);
    return false;
  }
  // little
  if (!rosidl_runtime_c__uint8__Sequence__init(&msg->little, 0)) {
    omnihand_2025_node_msgs__msg__TactileSensor__fini(msg);
    return false;
  }
  // palm
  if (!rosidl_runtime_c__uint8__Sequence__init(&msg->palm, 0)) {
    omnihand_2025_node_msgs__msg__TactileSensor__fini(msg);
    return false;
  }
  // dorsum
  if (!rosidl_runtime_c__uint8__Sequence__init(&msg->dorsum, 0)) {
    omnihand_2025_node_msgs__msg__TactileSensor__fini(msg);
    return false;
  }
  return true;
}

void
omnihand_2025_node_msgs__msg__TactileSensor__fini(omnihand_2025_node_msgs__msg__TactileSensor * msg)
{
  if (!msg) {
    return;
  }
  // header
  std_msgs__msg__Header__fini(&msg->header);
  // thumb
  rosidl_runtime_c__uint8__Sequence__fini(&msg->thumb);
  // index
  rosidl_runtime_c__uint8__Sequence__fini(&msg->index);
  // middle
  rosidl_runtime_c__uint8__Sequence__fini(&msg->middle);
  // ring
  rosidl_runtime_c__uint8__Sequence__fini(&msg->ring);
  // little
  rosidl_runtime_c__uint8__Sequence__fini(&msg->little);
  // palm
  rosidl_runtime_c__uint8__Sequence__fini(&msg->palm);
  // dorsum
  rosidl_runtime_c__uint8__Sequence__fini(&msg->dorsum);
}

bool
omnihand_2025_node_msgs__msg__TactileSensor__are_equal(const omnihand_2025_node_msgs__msg__TactileSensor * lhs, const omnihand_2025_node_msgs__msg__TactileSensor * rhs)
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
  // thumb
  if (!rosidl_runtime_c__uint8__Sequence__are_equal(
      &(lhs->thumb), &(rhs->thumb)))
  {
    return false;
  }
  // index
  if (!rosidl_runtime_c__uint8__Sequence__are_equal(
      &(lhs->index), &(rhs->index)))
  {
    return false;
  }
  // middle
  if (!rosidl_runtime_c__uint8__Sequence__are_equal(
      &(lhs->middle), &(rhs->middle)))
  {
    return false;
  }
  // ring
  if (!rosidl_runtime_c__uint8__Sequence__are_equal(
      &(lhs->ring), &(rhs->ring)))
  {
    return false;
  }
  // little
  if (!rosidl_runtime_c__uint8__Sequence__are_equal(
      &(lhs->little), &(rhs->little)))
  {
    return false;
  }
  // palm
  if (!rosidl_runtime_c__uint8__Sequence__are_equal(
      &(lhs->palm), &(rhs->palm)))
  {
    return false;
  }
  // dorsum
  if (!rosidl_runtime_c__uint8__Sequence__are_equal(
      &(lhs->dorsum), &(rhs->dorsum)))
  {
    return false;
  }
  return true;
}

bool
omnihand_2025_node_msgs__msg__TactileSensor__copy(
  const omnihand_2025_node_msgs__msg__TactileSensor * input,
  omnihand_2025_node_msgs__msg__TactileSensor * output)
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
  // thumb
  if (!rosidl_runtime_c__uint8__Sequence__copy(
      &(input->thumb), &(output->thumb)))
  {
    return false;
  }
  // index
  if (!rosidl_runtime_c__uint8__Sequence__copy(
      &(input->index), &(output->index)))
  {
    return false;
  }
  // middle
  if (!rosidl_runtime_c__uint8__Sequence__copy(
      &(input->middle), &(output->middle)))
  {
    return false;
  }
  // ring
  if (!rosidl_runtime_c__uint8__Sequence__copy(
      &(input->ring), &(output->ring)))
  {
    return false;
  }
  // little
  if (!rosidl_runtime_c__uint8__Sequence__copy(
      &(input->little), &(output->little)))
  {
    return false;
  }
  // palm
  if (!rosidl_runtime_c__uint8__Sequence__copy(
      &(input->palm), &(output->palm)))
  {
    return false;
  }
  // dorsum
  if (!rosidl_runtime_c__uint8__Sequence__copy(
      &(input->dorsum), &(output->dorsum)))
  {
    return false;
  }
  return true;
}

omnihand_2025_node_msgs__msg__TactileSensor *
omnihand_2025_node_msgs__msg__TactileSensor__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  omnihand_2025_node_msgs__msg__TactileSensor * msg = (omnihand_2025_node_msgs__msg__TactileSensor *)allocator.allocate(sizeof(omnihand_2025_node_msgs__msg__TactileSensor), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(omnihand_2025_node_msgs__msg__TactileSensor));
  bool success = omnihand_2025_node_msgs__msg__TactileSensor__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
omnihand_2025_node_msgs__msg__TactileSensor__destroy(omnihand_2025_node_msgs__msg__TactileSensor * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    omnihand_2025_node_msgs__msg__TactileSensor__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
omnihand_2025_node_msgs__msg__TactileSensor__Sequence__init(omnihand_2025_node_msgs__msg__TactileSensor__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  omnihand_2025_node_msgs__msg__TactileSensor * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(omnihand_2025_node_msgs__msg__TactileSensor)) {
      return false;
    }
    data = (omnihand_2025_node_msgs__msg__TactileSensor *)allocator.zero_allocate(size, sizeof(omnihand_2025_node_msgs__msg__TactileSensor), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = omnihand_2025_node_msgs__msg__TactileSensor__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        omnihand_2025_node_msgs__msg__TactileSensor__fini(&data[i - 1]);
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
omnihand_2025_node_msgs__msg__TactileSensor__Sequence__fini(omnihand_2025_node_msgs__msg__TactileSensor__Sequence * array)
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
      omnihand_2025_node_msgs__msg__TactileSensor__fini(&array->data[i]);
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

omnihand_2025_node_msgs__msg__TactileSensor__Sequence *
omnihand_2025_node_msgs__msg__TactileSensor__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  omnihand_2025_node_msgs__msg__TactileSensor__Sequence * array = (omnihand_2025_node_msgs__msg__TactileSensor__Sequence *)allocator.allocate(sizeof(omnihand_2025_node_msgs__msg__TactileSensor__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = omnihand_2025_node_msgs__msg__TactileSensor__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
omnihand_2025_node_msgs__msg__TactileSensor__Sequence__destroy(omnihand_2025_node_msgs__msg__TactileSensor__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    omnihand_2025_node_msgs__msg__TactileSensor__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
omnihand_2025_node_msgs__msg__TactileSensor__Sequence__are_equal(const omnihand_2025_node_msgs__msg__TactileSensor__Sequence * lhs, const omnihand_2025_node_msgs__msg__TactileSensor__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!omnihand_2025_node_msgs__msg__TactileSensor__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
omnihand_2025_node_msgs__msg__TactileSensor__Sequence__copy(
  const omnihand_2025_node_msgs__msg__TactileSensor__Sequence * input,
  omnihand_2025_node_msgs__msg__TactileSensor__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(omnihand_2025_node_msgs__msg__TactileSensor)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(omnihand_2025_node_msgs__msg__TactileSensor);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    omnihand_2025_node_msgs__msg__TactileSensor * data =
      (omnihand_2025_node_msgs__msg__TactileSensor *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!omnihand_2025_node_msgs__msg__TactileSensor__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          omnihand_2025_node_msgs__msg__TactileSensor__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!omnihand_2025_node_msgs__msg__TactileSensor__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
