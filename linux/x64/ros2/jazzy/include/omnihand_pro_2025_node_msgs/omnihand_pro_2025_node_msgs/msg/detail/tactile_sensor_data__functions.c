// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from omnihand_pro_2025_node_msgs:msg/TactileSensorData.idl
// generated code does not contain a copyright notice
#include "omnihand_pro_2025_node_msgs/msg/detail/tactile_sensor_data__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `channel_value`
// Member `capa_approach`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

bool
omnihand_pro_2025_node_msgs__msg__TactileSensorData__init(omnihand_pro_2025_node_msgs__msg__TactileSensorData * msg)
{
  if (!msg) {
    return false;
  }
  // online_state
  // channel_value
  if (!rosidl_runtime_c__uint32__Sequence__init(&msg->channel_value, 0)) {
    omnihand_pro_2025_node_msgs__msg__TactileSensorData__fini(msg);
    return false;
  }
  // normal_force
  // tangent_force
  // tangent_force_angle
  // capa_approach
  if (!rosidl_runtime_c__uint8__Sequence__init(&msg->capa_approach, 0)) {
    omnihand_pro_2025_node_msgs__msg__TactileSensorData__fini(msg);
    return false;
  }
  return true;
}

void
omnihand_pro_2025_node_msgs__msg__TactileSensorData__fini(omnihand_pro_2025_node_msgs__msg__TactileSensorData * msg)
{
  if (!msg) {
    return;
  }
  // online_state
  // channel_value
  rosidl_runtime_c__uint32__Sequence__fini(&msg->channel_value);
  // normal_force
  // tangent_force
  // tangent_force_angle
  // capa_approach
  rosidl_runtime_c__uint8__Sequence__fini(&msg->capa_approach);
}

bool
omnihand_pro_2025_node_msgs__msg__TactileSensorData__are_equal(const omnihand_pro_2025_node_msgs__msg__TactileSensorData * lhs, const omnihand_pro_2025_node_msgs__msg__TactileSensorData * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // online_state
  if (lhs->online_state != rhs->online_state) {
    return false;
  }
  // channel_value
  if (!rosidl_runtime_c__uint32__Sequence__are_equal(
      &(lhs->channel_value), &(rhs->channel_value)))
  {
    return false;
  }
  // normal_force
  if (lhs->normal_force != rhs->normal_force) {
    return false;
  }
  // tangent_force
  if (lhs->tangent_force != rhs->tangent_force) {
    return false;
  }
  // tangent_force_angle
  if (lhs->tangent_force_angle != rhs->tangent_force_angle) {
    return false;
  }
  // capa_approach
  if (!rosidl_runtime_c__uint8__Sequence__are_equal(
      &(lhs->capa_approach), &(rhs->capa_approach)))
  {
    return false;
  }
  return true;
}

bool
omnihand_pro_2025_node_msgs__msg__TactileSensorData__copy(
  const omnihand_pro_2025_node_msgs__msg__TactileSensorData * input,
  omnihand_pro_2025_node_msgs__msg__TactileSensorData * output)
{
  if (!input || !output) {
    return false;
  }
  // online_state
  output->online_state = input->online_state;
  // channel_value
  if (!rosidl_runtime_c__uint32__Sequence__copy(
      &(input->channel_value), &(output->channel_value)))
  {
    return false;
  }
  // normal_force
  output->normal_force = input->normal_force;
  // tangent_force
  output->tangent_force = input->tangent_force;
  // tangent_force_angle
  output->tangent_force_angle = input->tangent_force_angle;
  // capa_approach
  if (!rosidl_runtime_c__uint8__Sequence__copy(
      &(input->capa_approach), &(output->capa_approach)))
  {
    return false;
  }
  return true;
}

omnihand_pro_2025_node_msgs__msg__TactileSensorData *
omnihand_pro_2025_node_msgs__msg__TactileSensorData__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  omnihand_pro_2025_node_msgs__msg__TactileSensorData * msg = (omnihand_pro_2025_node_msgs__msg__TactileSensorData *)allocator.allocate(sizeof(omnihand_pro_2025_node_msgs__msg__TactileSensorData), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(omnihand_pro_2025_node_msgs__msg__TactileSensorData));
  bool success = omnihand_pro_2025_node_msgs__msg__TactileSensorData__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
omnihand_pro_2025_node_msgs__msg__TactileSensorData__destroy(omnihand_pro_2025_node_msgs__msg__TactileSensorData * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    omnihand_pro_2025_node_msgs__msg__TactileSensorData__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
omnihand_pro_2025_node_msgs__msg__TactileSensorData__Sequence__init(omnihand_pro_2025_node_msgs__msg__TactileSensorData__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  omnihand_pro_2025_node_msgs__msg__TactileSensorData * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(omnihand_pro_2025_node_msgs__msg__TactileSensorData)) {
      return false;
    }
    data = (omnihand_pro_2025_node_msgs__msg__TactileSensorData *)allocator.zero_allocate(size, sizeof(omnihand_pro_2025_node_msgs__msg__TactileSensorData), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = omnihand_pro_2025_node_msgs__msg__TactileSensorData__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        omnihand_pro_2025_node_msgs__msg__TactileSensorData__fini(&data[i - 1]);
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
omnihand_pro_2025_node_msgs__msg__TactileSensorData__Sequence__fini(omnihand_pro_2025_node_msgs__msg__TactileSensorData__Sequence * array)
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
      omnihand_pro_2025_node_msgs__msg__TactileSensorData__fini(&array->data[i]);
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

omnihand_pro_2025_node_msgs__msg__TactileSensorData__Sequence *
omnihand_pro_2025_node_msgs__msg__TactileSensorData__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  omnihand_pro_2025_node_msgs__msg__TactileSensorData__Sequence * array = (omnihand_pro_2025_node_msgs__msg__TactileSensorData__Sequence *)allocator.allocate(sizeof(omnihand_pro_2025_node_msgs__msg__TactileSensorData__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = omnihand_pro_2025_node_msgs__msg__TactileSensorData__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
omnihand_pro_2025_node_msgs__msg__TactileSensorData__Sequence__destroy(omnihand_pro_2025_node_msgs__msg__TactileSensorData__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    omnihand_pro_2025_node_msgs__msg__TactileSensorData__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
omnihand_pro_2025_node_msgs__msg__TactileSensorData__Sequence__are_equal(const omnihand_pro_2025_node_msgs__msg__TactileSensorData__Sequence * lhs, const omnihand_pro_2025_node_msgs__msg__TactileSensorData__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!omnihand_pro_2025_node_msgs__msg__TactileSensorData__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
omnihand_pro_2025_node_msgs__msg__TactileSensorData__Sequence__copy(
  const omnihand_pro_2025_node_msgs__msg__TactileSensorData__Sequence * input,
  omnihand_pro_2025_node_msgs__msg__TactileSensorData__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(omnihand_pro_2025_node_msgs__msg__TactileSensorData)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(omnihand_pro_2025_node_msgs__msg__TactileSensorData);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    omnihand_pro_2025_node_msgs__msg__TactileSensorData * data =
      (omnihand_pro_2025_node_msgs__msg__TactileSensorData *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!omnihand_pro_2025_node_msgs__msg__TactileSensorData__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          omnihand_pro_2025_node_msgs__msg__TactileSensorData__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!omnihand_pro_2025_node_msgs__msg__TactileSensorData__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
