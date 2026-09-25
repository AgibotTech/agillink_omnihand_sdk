#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};


#[link(name = "omnihand_node_msgs__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__omnihand_node_msgs__msg__Int8MultiArrayStamped() -> *const std::ffi::c_void;
}

#[link(name = "omnihand_node_msgs__rosidl_generator_c")]
extern "C" {
    fn omnihand_node_msgs__msg__Int8MultiArrayStamped__init(msg: *mut Int8MultiArrayStamped) -> bool;
    fn omnihand_node_msgs__msg__Int8MultiArrayStamped__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<Int8MultiArrayStamped>, size: usize) -> bool;
    fn omnihand_node_msgs__msg__Int8MultiArrayStamped__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<Int8MultiArrayStamped>);
    fn omnihand_node_msgs__msg__Int8MultiArrayStamped__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<Int8MultiArrayStamped>, out_seq: *mut rosidl_runtime_rs::Sequence<Int8MultiArrayStamped>) -> bool;
}

// Corresponds to omnihand_node_msgs__msg__Int8MultiArrayStamped
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct Int8MultiArrayStamped {

    // This member is not documented.
    #[allow(missing_docs)]
    pub header: std_msgs::msg::rmw::Header,


    // This member is not documented.
    #[allow(missing_docs)]
    pub layout: std_msgs::msg::rmw::MultiArrayLayout,


    // This member is not documented.
    #[allow(missing_docs)]
    pub data: rosidl_runtime_rs::Sequence<i8>,

}



impl Default for Int8MultiArrayStamped {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !omnihand_node_msgs__msg__Int8MultiArrayStamped__init(&mut msg as *mut _) {
        panic!("Call to omnihand_node_msgs__msg__Int8MultiArrayStamped__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for Int8MultiArrayStamped {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { omnihand_node_msgs__msg__Int8MultiArrayStamped__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { omnihand_node_msgs__msg__Int8MultiArrayStamped__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { omnihand_node_msgs__msg__Int8MultiArrayStamped__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for Int8MultiArrayStamped {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for Int8MultiArrayStamped where Self: Sized {
  const TYPE_NAME: &'static str = "omnihand_node_msgs/msg/Int8MultiArrayStamped";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__omnihand_node_msgs__msg__Int8MultiArrayStamped() }
  }
}


#[link(name = "omnihand_node_msgs__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__omnihand_node_msgs__msg__Int16MultiArrayStamped() -> *const std::ffi::c_void;
}

#[link(name = "omnihand_node_msgs__rosidl_generator_c")]
extern "C" {
    fn omnihand_node_msgs__msg__Int16MultiArrayStamped__init(msg: *mut Int16MultiArrayStamped) -> bool;
    fn omnihand_node_msgs__msg__Int16MultiArrayStamped__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<Int16MultiArrayStamped>, size: usize) -> bool;
    fn omnihand_node_msgs__msg__Int16MultiArrayStamped__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<Int16MultiArrayStamped>);
    fn omnihand_node_msgs__msg__Int16MultiArrayStamped__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<Int16MultiArrayStamped>, out_seq: *mut rosidl_runtime_rs::Sequence<Int16MultiArrayStamped>) -> bool;
}

// Corresponds to omnihand_node_msgs__msg__Int16MultiArrayStamped
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct Int16MultiArrayStamped {

    // This member is not documented.
    #[allow(missing_docs)]
    pub header: std_msgs::msg::rmw::Header,


    // This member is not documented.
    #[allow(missing_docs)]
    pub layout: std_msgs::msg::rmw::MultiArrayLayout,


    // This member is not documented.
    #[allow(missing_docs)]
    pub data: rosidl_runtime_rs::Sequence<i16>,

}



impl Default for Int16MultiArrayStamped {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !omnihand_node_msgs__msg__Int16MultiArrayStamped__init(&mut msg as *mut _) {
        panic!("Call to omnihand_node_msgs__msg__Int16MultiArrayStamped__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for Int16MultiArrayStamped {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { omnihand_node_msgs__msg__Int16MultiArrayStamped__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { omnihand_node_msgs__msg__Int16MultiArrayStamped__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { omnihand_node_msgs__msg__Int16MultiArrayStamped__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for Int16MultiArrayStamped {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for Int16MultiArrayStamped where Self: Sized {
  const TYPE_NAME: &'static str = "omnihand_node_msgs/msg/Int16MultiArrayStamped";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__omnihand_node_msgs__msg__Int16MultiArrayStamped() }
  }
}


