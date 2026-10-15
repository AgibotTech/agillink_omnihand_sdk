#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};


#[link(name = "omnipicker_3_node_msgs__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__omnipicker_3_node_msgs__msg__GentleGripCommand() -> *const std::ffi::c_void;
}

#[link(name = "omnipicker_3_node_msgs__rosidl_generator_c")]
extern "C" {
    fn omnipicker_3_node_msgs__msg__GentleGripCommand__init(msg: *mut GentleGripCommand) -> bool;
    fn omnipicker_3_node_msgs__msg__GentleGripCommand__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<GentleGripCommand>, size: usize) -> bool;
    fn omnipicker_3_node_msgs__msg__GentleGripCommand__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<GentleGripCommand>);
    fn omnipicker_3_node_msgs__msg__GentleGripCommand__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<GentleGripCommand>, out_seq: *mut rosidl_runtime_rs::Sequence<GentleGripCommand>) -> bool;
}

// Corresponds to omnipicker_3_node_msgs__msg__GentleGripCommand
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]

/// Enter gentle-grip mode, optionally applying an additional current limit.

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct GentleGripCommand {

    // This member is not documented.
    #[allow(missing_docs)]
    pub use_current_limit: bool,


    // This member is not documented.
    #[allow(missing_docs)]
    pub current_limit_ma: u16,

}



impl Default for GentleGripCommand {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !omnipicker_3_node_msgs__msg__GentleGripCommand__init(&mut msg as *mut _) {
        panic!("Call to omnipicker_3_node_msgs__msg__GentleGripCommand__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for GentleGripCommand {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { omnipicker_3_node_msgs__msg__GentleGripCommand__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { omnipicker_3_node_msgs__msg__GentleGripCommand__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { omnipicker_3_node_msgs__msg__GentleGripCommand__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for GentleGripCommand {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for GentleGripCommand where Self: Sized {
  const TYPE_NAME: &'static str = "omnipicker_3_node_msgs/msg/GentleGripCommand";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__omnipicker_3_node_msgs__msg__GentleGripCommand() }
  }
}


#[link(name = "omnipicker_3_node_msgs__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__omnipicker_3_node_msgs__msg__GentleGripState() -> *const std::ffi::c_void;
}

#[link(name = "omnipicker_3_node_msgs__rosidl_generator_c")]
extern "C" {
    fn omnipicker_3_node_msgs__msg__GentleGripState__init(msg: *mut GentleGripState) -> bool;
    fn omnipicker_3_node_msgs__msg__GentleGripState__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<GentleGripState>, size: usize) -> bool;
    fn omnipicker_3_node_msgs__msg__GentleGripState__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<GentleGripState>);
    fn omnipicker_3_node_msgs__msg__GentleGripState__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<GentleGripState>, out_seq: *mut rosidl_runtime_rs::Sequence<GentleGripState>) -> bool;
}

// Corresponds to omnipicker_3_node_msgs__msg__GentleGripState
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct GentleGripState {

    // This member is not documented.
    #[allow(missing_docs)]
    pub header: std_msgs::msg::rmw::Header,


    // This member is not documented.
    #[allow(missing_docs)]
    pub raw_response: rosidl_runtime_rs::Sequence<u8>,

}



impl Default for GentleGripState {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !omnipicker_3_node_msgs__msg__GentleGripState__init(&mut msg as *mut _) {
        panic!("Call to omnipicker_3_node_msgs__msg__GentleGripState__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for GentleGripState {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { omnipicker_3_node_msgs__msg__GentleGripState__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { omnipicker_3_node_msgs__msg__GentleGripState__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { omnipicker_3_node_msgs__msg__GentleGripState__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for GentleGripState {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for GentleGripState where Self: Sized {
  const TYPE_NAME: &'static str = "omnipicker_3_node_msgs/msg/GentleGripState";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__omnipicker_3_node_msgs__msg__GentleGripState() }
  }
}


#[link(name = "omnipicker_3_node_msgs__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__omnipicker_3_node_msgs__msg__StrokeCommand() -> *const std::ffi::c_void;
}

#[link(name = "omnipicker_3_node_msgs__rosidl_generator_c")]
extern "C" {
    fn omnipicker_3_node_msgs__msg__StrokeCommand__init(msg: *mut StrokeCommand) -> bool;
    fn omnipicker_3_node_msgs__msg__StrokeCommand__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<StrokeCommand>, size: usize) -> bool;
    fn omnipicker_3_node_msgs__msg__StrokeCommand__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<StrokeCommand>);
    fn omnipicker_3_node_msgs__msg__StrokeCommand__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<StrokeCommand>, out_seq: *mut rosidl_runtime_rs::Sequence<StrokeCommand>) -> bool;
}

// Corresponds to omnipicker_3_node_msgs__msg__StrokeCommand
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]

/// OP3 distance control. stroke_mm is millimetres; speed/current use SDK units.

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct StrokeCommand {

    // This member is not documented.
    #[allow(missing_docs)]
    pub stroke_mm: f64,


    // This member is not documented.
    #[allow(missing_docs)]
    pub use_speed: bool,


    // This member is not documented.
    #[allow(missing_docs)]
    pub speed_rpm: i16,


    // This member is not documented.
    #[allow(missing_docs)]
    pub use_current: bool,


    // This member is not documented.
    #[allow(missing_docs)]
    pub current_ma: i16,

}



impl Default for StrokeCommand {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !omnipicker_3_node_msgs__msg__StrokeCommand__init(&mut msg as *mut _) {
        panic!("Call to omnipicker_3_node_msgs__msg__StrokeCommand__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for StrokeCommand {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { omnipicker_3_node_msgs__msg__StrokeCommand__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { omnipicker_3_node_msgs__msg__StrokeCommand__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { omnipicker_3_node_msgs__msg__StrokeCommand__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for StrokeCommand {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for StrokeCommand where Self: Sized {
  const TYPE_NAME: &'static str = "omnipicker_3_node_msgs/msg/StrokeCommand";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__omnipicker_3_node_msgs__msg__StrokeCommand() }
  }
}


#[link(name = "omnipicker_3_node_msgs__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__omnipicker_3_node_msgs__msg__StrokeState() -> *const std::ffi::c_void;
}

#[link(name = "omnipicker_3_node_msgs__rosidl_generator_c")]
extern "C" {
    fn omnipicker_3_node_msgs__msg__StrokeState__init(msg: *mut StrokeState) -> bool;
    fn omnipicker_3_node_msgs__msg__StrokeState__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<StrokeState>, size: usize) -> bool;
    fn omnipicker_3_node_msgs__msg__StrokeState__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<StrokeState>);
    fn omnipicker_3_node_msgs__msg__StrokeState__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<StrokeState>, out_seq: *mut rosidl_runtime_rs::Sequence<StrokeState>) -> bool;
}

// Corresponds to omnipicker_3_node_msgs__msg__StrokeState
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct StrokeState {

    // This member is not documented.
    #[allow(missing_docs)]
    pub header: std_msgs::msg::rmw::Header,


    // This member is not documented.
    #[allow(missing_docs)]
    pub stroke_mm: f64,

}



impl Default for StrokeState {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !omnipicker_3_node_msgs__msg__StrokeState__init(&mut msg as *mut _) {
        panic!("Call to omnipicker_3_node_msgs__msg__StrokeState__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for StrokeState {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { omnipicker_3_node_msgs__msg__StrokeState__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { omnipicker_3_node_msgs__msg__StrokeState__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { omnipicker_3_node_msgs__msg__StrokeState__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for StrokeState {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for StrokeState where Self: Sized {
  const TYPE_NAME: &'static str = "omnipicker_3_node_msgs/msg/StrokeState";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__omnipicker_3_node_msgs__msg__StrokeState() }
  }
}


#[link(name = "omnipicker_3_node_msgs__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__omnipicker_3_node_msgs__msg__TactileSensor() -> *const std::ffi::c_void;
}

#[link(name = "omnipicker_3_node_msgs__rosidl_generator_c")]
extern "C" {
    fn omnipicker_3_node_msgs__msg__TactileSensor__init(msg: *mut TactileSensor) -> bool;
    fn omnipicker_3_node_msgs__msg__TactileSensor__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<TactileSensor>, size: usize) -> bool;
    fn omnipicker_3_node_msgs__msg__TactileSensor__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<TactileSensor>);
    fn omnipicker_3_node_msgs__msg__TactileSensor__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<TactileSensor>, out_seq: *mut rosidl_runtime_rs::Sequence<TactileSensor>) -> bool;
}

// Corresponds to omnipicker_3_node_msgs__msg__TactileSensor
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]

/// OmniPicker 3 (OP3) 16-bit 1D tactile data from its two sensor regions.

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

}



impl Default for TactileSensor {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !omnipicker_3_node_msgs__msg__TactileSensor__init(&mut msg as *mut _) {
        panic!("Call to omnipicker_3_node_msgs__msg__TactileSensor__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for TactileSensor {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { omnipicker_3_node_msgs__msg__TactileSensor__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { omnipicker_3_node_msgs__msg__TactileSensor__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { omnipicker_3_node_msgs__msg__TactileSensor__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for TactileSensor {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for TactileSensor where Self: Sized {
  const TYPE_NAME: &'static str = "omnipicker_3_node_msgs/msg/TactileSensor";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__omnipicker_3_node_msgs__msg__TactileSensor() }
  }
}


