// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from omnipicker_3_node_msgs:msg/StrokeState.idl
// generated code does not contain a copyright notice
#include "omnipicker_3_node_msgs/msg/detail/stroke_state__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/detail/header__functions.h"

bool
omnipicker_3_node_msgs__msg__StrokeState__init(omnipicker_3_node_msgs__msg__StrokeState * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__init(&msg->header)) {
    omnipicker_3_node_msgs__msg__StrokeState__fini(msg);
    return false;
  }
  // stroke_mm
  return true;
}

void
omnipicker_3_node_msgs__msg__StrokeState__fini(omnipicker_3_node_msgs__msg__StrokeState * msg)
{
  if (!msg) {
    return;
  }
  // header
  std_msgs__msg__Header__fini(&msg->header);
  // stroke_mm
}

bool
omnipicker_3_node_msgs__msg__StrokeState__are_equal(const omnipicker_3_node_msgs__msg__StrokeState * lhs, const omnipicker_3_node_msgs__msg__StrokeState * rhs)
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
  // stroke_mm
  if (lhs->stroke_mm != rhs->stroke_mm) {
    return false;
  }
  return true;
}

bool
omnipicker_3_node_msgs__msg__StrokeState__copy(
  const omnipicker_3_node_msgs__msg__StrokeState * input,
  omnipicker_3_node_msgs__msg__StrokeState * output)
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
  // stroke_mm
  output->stroke_mm = input->stroke_mm;
  return true;
}

omnipicker_3_node_msgs__msg__StrokeState *
omnipicker_3_node_msgs__msg__StrokeState__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  omnipicker_3_node_msgs__msg__StrokeState * msg = (omnipicker_3_node_msgs__msg__StrokeState *)allocator.allocate(sizeof(omnipicker_3_node_msgs__msg__StrokeState), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(omnipicker_3_node_msgs__msg__StrokeState));
  bool success = omnipicker_3_node_msgs__msg__StrokeState__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
omnipicker_3_node_msgs__msg__StrokeState__destroy(omnipicker_3_node_msgs__msg__StrokeState * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    omnipicker_3_node_msgs__msg__StrokeState__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
omnipicker_3_node_msgs__msg__StrokeState__Sequence__init(omnipicker_3_node_msgs__msg__StrokeState__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  omnipicker_3_node_msgs__msg__StrokeState * data = NULL;

  if (size) {
    data = (omnipicker_3_node_msgs__msg__StrokeState *)allocator.zero_allocate(size, sizeof(omnipicker_3_node_msgs__msg__StrokeState), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = omnipicker_3_node_msgs__msg__StrokeState__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        omnipicker_3_node_msgs__msg__StrokeState__fini(&data[i - 1]);
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
omnipicker_3_node_msgs__msg__StrokeState__Sequence__fini(omnipicker_3_node_msgs__msg__StrokeState__Sequence * array)
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
      omnipicker_3_node_msgs__msg__StrokeState__fini(&array->data[i]);
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

omnipicker_3_node_msgs__msg__StrokeState__Sequence *
omnipicker_3_node_msgs__msg__StrokeState__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  omnipicker_3_node_msgs__msg__StrokeState__Sequence * array = (omnipicker_3_node_msgs__msg__StrokeState__Sequence *)allocator.allocate(sizeof(omnipicker_3_node_msgs__msg__StrokeState__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = omnipicker_3_node_msgs__msg__StrokeState__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
omnipicker_3_node_msgs__msg__StrokeState__Sequence__destroy(omnipicker_3_node_msgs__msg__StrokeState__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    omnipicker_3_node_msgs__msg__StrokeState__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
omnipicker_3_node_msgs__msg__StrokeState__Sequence__are_equal(const omnipicker_3_node_msgs__msg__StrokeState__Sequence * lhs, const omnipicker_3_node_msgs__msg__StrokeState__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!omnipicker_3_node_msgs__msg__StrokeState__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
omnipicker_3_node_msgs__msg__StrokeState__Sequence__copy(
  const omnipicker_3_node_msgs__msg__StrokeState__Sequence * input,
  omnipicker_3_node_msgs__msg__StrokeState__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(omnipicker_3_node_msgs__msg__StrokeState);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    omnipicker_3_node_msgs__msg__StrokeState * data =
      (omnipicker_3_node_msgs__msg__StrokeState *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!omnipicker_3_node_msgs__msg__StrokeState__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          omnipicker_3_node_msgs__msg__StrokeState__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!omnipicker_3_node_msgs__msg__StrokeState__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
