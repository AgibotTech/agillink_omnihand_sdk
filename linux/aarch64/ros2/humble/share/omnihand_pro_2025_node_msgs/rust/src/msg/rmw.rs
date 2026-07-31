#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};


#[link(name = "omnihand_pro_2025_node_msgs__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__omnihand_pro_2025_node_msgs__msg__TactileSensorData() -> *const std::ffi::c_void;
}

#[link(name = "omnihand_pro_2025_node_msgs__rosidl_generator_c")]
extern "C" {
    fn omnihand_pro_2025_node_msgs__msg__TactileSensorData__init(msg: *mut TactileSensorData) -> bool;
    fn omnihand_pro_2025_node_msgs__msg__TactileSensorData__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<TactileSensorData>, size: usize) -> bool;
    fn omnihand_pro_2025_node_msgs__msg__TactileSensorData__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<TactileSensorData>);
    fn omnihand_pro_2025_node_msgs__msg__TactileSensorData__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<TactileSensorData>, out_seq: *mut rosidl_runtime_rs::Sequence<TactileSensorData>) -> bool;
}

// Corresponds to omnihand_pro_2025_node_msgs__msg__TactileSensorData
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct TactileSensorData {
    /// 1=sensor online; 0=sensor offline
    pub online_state: u8,

    /// 6-channel 24-bit value
    pub channel_value: rosidl_runtime_rs::Sequence<u32>,

    /// normal force (0.1 N, 0 ~ 3000)
    pub normal_force: u16,

    /// tangent force (0.1 N, 0 ~ 3000)
    pub tangent_force: u16,

    /// tangent force angle, fingertip-up = 0°, clockwise (0~359)
    pub tangent_force_angle: u16,

    /// 4 self-capacitance proximity values
    pub capa_approach: rosidl_runtime_rs::Sequence<u8>,

}



impl Default for TactileSensorData {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !omnihand_pro_2025_node_msgs__msg__TactileSensorData__init(&mut msg as *mut _) {
        panic!("Call to omnihand_pro_2025_node_msgs__msg__TactileSensorData__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for TactileSensorData {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { omnihand_pro_2025_node_msgs__msg__TactileSensorData__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { omnihand_pro_2025_node_msgs__msg__TactileSensorData__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { omnihand_pro_2025_node_msgs__msg__TactileSensorData__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for TactileSensorData {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for TactileSensorData where Self: Sized {
  const TYPE_NAME: &'static str = "omnihand_pro_2025_node_msgs/msg/TactileSensorData";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__omnihand_pro_2025_node_msgs__msg__TactileSensorData() }
  }
}


#[link(name = "omnihand_pro_2025_node_msgs__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__omnihand_pro_2025_node_msgs__msg__TactileSensor() -> *const std::ffi::c_void;
}

#[link(name = "omnihand_pro_2025_node_msgs__rosidl_generator_c")]
extern "C" {
    fn omnihand_pro_2025_node_msgs__msg__TactileSensor__init(msg: *mut TactileSensor) -> bool;
    fn omnihand_pro_2025_node_msgs__msg__TactileSensor__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<TactileSensor>, size: usize) -> bool;
    fn omnihand_pro_2025_node_msgs__msg__TactileSensor__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<TactileSensor>);
    fn omnihand_pro_2025_node_msgs__msg__TactileSensor__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<TactileSensor>, out_seq: *mut rosidl_runtime_rs::Sequence<TactileSensor>) -> bool;
}

// Corresponds to omnihand_pro_2025_node_msgs__msg__TactileSensor
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]

/// OmniHand Pro 2025 (O12) 3D tactile: one TactileSensorData per finger (thumb … palm).

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct TactileSensor {

    // This member is not documented.
    #[allow(missing_docs)]
    pub header: std_msgs::msg::rmw::Header,


    // This member is not documented.
    #[allow(missing_docs)]
    pub thumb: super::super::msg::rmw::TactileSensorData,


    // This member is not documented.
    #[allow(missing_docs)]
    pub index: super::super::msg::rmw::TactileSensorData,


    // This member is not documented.
    #[allow(missing_docs)]
    pub middle: super::super::msg::rmw::TactileSensorData,


    // This member is not documented.
    #[allow(missing_docs)]
    pub ring: super::super::msg::rmw::TactileSensorData,


    // This member is not documented.
    #[allow(missing_docs)]
    pub little: super::super::msg::rmw::TactileSensorData,


    // This member is not documented.
    #[allow(missing_docs)]
    pub palm: super::super::msg::rmw::TactileSensorData,

}



impl Default for TactileSensor {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !omnihand_pro_2025_node_msgs__msg__TactileSensor__init(&mut msg as *mut _) {
        panic!("Call to omnihand_pro_2025_node_msgs__msg__TactileSensor__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for TactileSensor {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { omnihand_pro_2025_node_msgs__msg__TactileSensor__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { omnihand_pro_2025_node_msgs__msg__TactileSensor__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { omnihand_pro_2025_node_msgs__msg__TactileSensor__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for TactileSensor {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for TactileSensor where Self: Sized {
  const TYPE_NAME: &'static str = "omnihand_pro_2025_node_msgs/msg/TactileSensor";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__omnihand_pro_2025_node_msgs__msg__TactileSensor() }
  }
}


