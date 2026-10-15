#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};


#[link(name = "omnihand_3_lite_node_msgs__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__omnihand_3_lite_node_msgs__msg__TactileSensor() -> *const std::ffi::c_void;
}

#[link(name = "omnihand_3_lite_node_msgs__rosidl_generator_c")]
extern "C" {
    fn omnihand_3_lite_node_msgs__msg__TactileSensor__init(msg: *mut TactileSensor) -> bool;
    fn omnihand_3_lite_node_msgs__msg__TactileSensor__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<TactileSensor>, size: usize) -> bool;
    fn omnihand_3_lite_node_msgs__msg__TactileSensor__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<TactileSensor>);
    fn omnihand_3_lite_node_msgs__msg__TactileSensor__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<TactileSensor>, out_seq: *mut rosidl_runtime_rs::Sequence<TactileSensor>) -> bool;
}

// Corresponds to omnihand_3_lite_node_msgs__msg__TactileSensor
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]

/// OmniHand 3 Lite (H3L) 16-bit 1D tactile data from six sensor regions.

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct TactileSensor {

    // This member is not documented.
    #[allow(missing_docs)]
    pub header: std_msgs::msg::rmw::Header,


    // This member is not documented.
    #[allow(missing_docs)]
    pub thumb: rosidl_runtime_rs::Sequence<u16>,


    // This member is not documented.
    #[allow(missing_docs)]
    pub index: rosidl_runtime_rs::Sequence<u16>,


    // This member is not documented.
    #[allow(missing_docs)]
    pub middle: rosidl_runtime_rs::Sequence<u16>,


    // This member is not documented.
    #[allow(missing_docs)]
    pub ring: rosidl_runtime_rs::Sequence<u16>,


    // This member is not documented.
    #[allow(missing_docs)]
    pub little: rosidl_runtime_rs::Sequence<u16>,


    // This member is not documented.
    #[allow(missing_docs)]
    pub palm: rosidl_runtime_rs::Sequence<u16>,

}



impl Default for TactileSensor {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !omnihand_3_lite_node_msgs__msg__TactileSensor__init(&mut msg as *mut _) {
        panic!("Call to omnihand_3_lite_node_msgs__msg__TactileSensor__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for TactileSensor {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { omnihand_3_lite_node_msgs__msg__TactileSensor__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { omnihand_3_lite_node_msgs__msg__TactileSensor__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { omnihand_3_lite_node_msgs__msg__TactileSensor__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for TactileSensor {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for TactileSensor where Self: Sized {
  const TYPE_NAME: &'static str = "omnihand_3_lite_node_msgs/msg/TactileSensor";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__omnihand_3_lite_node_msgs__msg__TactileSensor() }
  }
}


