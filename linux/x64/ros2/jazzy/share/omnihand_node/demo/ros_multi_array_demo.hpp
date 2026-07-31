#ifndef AGILINK_OMNIHAND_ROS2_DEMO_MULTI_ARRAY_HPP
#define AGILINK_OMNIHAND_ROS2_DEMO_MULTI_ARRAY_HPP

#include <cstdint>
#include <vector>

#include "std_msgs/msg/int16_multi_array.hpp"

namespace agilink {
namespace omnihand {
namespace ros2 {

inline std_msgs::msg::Int16MultiArray PackInt16MultiArray1D(const std::vector<int16_t>& data) {
  std_msgs::msg::Int16MultiArray out;
  out.layout.data_offset = 0;
  if (!data.empty()) {
    std_msgs::msg::MultiArrayDimension dim;
    dim.label = "joints";
    dim.size = static_cast<uint32_t>(data.size());
    dim.stride = static_cast<uint32_t>(data.size());
    out.layout.dim.push_back(dim);
  }
  out.data = data;
  return out;
}

}  // namespace ros2
}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OMNIHAND_ROS2_DEMO_MULTI_ARRAY_HPP
