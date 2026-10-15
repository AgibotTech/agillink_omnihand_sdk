#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};



// Corresponds to omnipicker_3_node_msgs__msg__GentleGripCommand
/// Enter gentle-grip mode, optionally applying an additional current limit.

#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
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
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::msg::rmw::GentleGripCommand::default())
  }
}

impl rosidl_runtime_rs::Message for GentleGripCommand {
  type RmwMsg = super::msg::rmw::GentleGripCommand;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        use_current_limit: msg.use_current_limit,
        current_limit_ma: msg.current_limit_ma,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
      use_current_limit: msg.use_current_limit,
      current_limit_ma: msg.current_limit_ma,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      use_current_limit: msg.use_current_limit,
      current_limit_ma: msg.current_limit_ma,
    }
  }
}


// Corresponds to omnipicker_3_node_msgs__msg__GentleGripState

// This struct is not documented.
#[allow(missing_docs)]

#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct GentleGripState {

    // This member is not documented.
    #[allow(missing_docs)]
    pub header: std_msgs::msg::Header,


    // This member is not documented.
    #[allow(missing_docs)]
    pub raw_response: Vec<u8>,

}



impl Default for GentleGripState {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::msg::rmw::GentleGripState::default())
  }
}

impl rosidl_runtime_rs::Message for GentleGripState {
  type RmwMsg = super::msg::rmw::GentleGripState;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        header: std_msgs::msg::Header::into_rmw_message(std::borrow::Cow::Owned(msg.header)).into_owned(),
        raw_response: msg.raw_response.into(),
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        header: std_msgs::msg::Header::into_rmw_message(std::borrow::Cow::Borrowed(&msg.header)).into_owned(),
        raw_response: msg.raw_response.as_slice().into(),
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      header: std_msgs::msg::Header::from_rmw_message(msg.header),
      raw_response: msg.raw_response
          .into_iter()
          .collect(),
    }
  }
}


// Corresponds to omnipicker_3_node_msgs__msg__StrokeCommand
/// OP3 distance control. stroke_mm is millimetres; speed/current use SDK units.

#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
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
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::msg::rmw::StrokeCommand::default())
  }
}

impl rosidl_runtime_rs::Message for StrokeCommand {
  type RmwMsg = super::msg::rmw::StrokeCommand;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        stroke_mm: msg.stroke_mm,
        use_speed: msg.use_speed,
        speed_rpm: msg.speed_rpm,
        use_current: msg.use_current,
        current_ma: msg.current_ma,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
      stroke_mm: msg.stroke_mm,
      use_speed: msg.use_speed,
      speed_rpm: msg.speed_rpm,
      use_current: msg.use_current,
      current_ma: msg.current_ma,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      stroke_mm: msg.stroke_mm,
      use_speed: msg.use_speed,
      speed_rpm: msg.speed_rpm,
      use_current: msg.use_current,
      current_ma: msg.current_ma,
    }
  }
}


// Corresponds to omnipicker_3_node_msgs__msg__StrokeState

// This struct is not documented.
#[allow(missing_docs)]

#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct StrokeState {

    // This member is not documented.
    #[allow(missing_docs)]
    pub header: std_msgs::msg::Header,


    // This member is not documented.
    #[allow(missing_docs)]
    pub stroke_mm: f64,

}



impl Default for StrokeState {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::msg::rmw::StrokeState::default())
  }
}

impl rosidl_runtime_rs::Message for StrokeState {
  type RmwMsg = super::msg::rmw::StrokeState;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        header: std_msgs::msg::Header::into_rmw_message(std::borrow::Cow::Owned(msg.header)).into_owned(),
        stroke_mm: msg.stroke_mm,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        header: std_msgs::msg::Header::into_rmw_message(std::borrow::Cow::Borrowed(&msg.header)).into_owned(),
      stroke_mm: msg.stroke_mm,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      header: std_msgs::msg::Header::from_rmw_message(msg.header),
      stroke_mm: msg.stroke_mm,
    }
  }
}


// Corresponds to omnipicker_3_node_msgs__msg__TactileSensor
/// OmniPicker 3 (OP3) 16-bit 1D tactile data from its two sensor regions.

#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct TactileSensor {

    // This member is not documented.
    #[allow(missing_docs)]
    pub header: std_msgs::msg::Header,


    // This member is not documented.
    #[allow(missing_docs)]
    pub thumb: Vec<u16>,


    // This member is not documented.
    #[allow(missing_docs)]
    pub index: Vec<u16>,

}



impl Default for TactileSensor {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::msg::rmw::TactileSensor::default())
  }
}

impl rosidl_runtime_rs::Message for TactileSensor {
  type RmwMsg = super::msg::rmw::TactileSensor;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        header: std_msgs::msg::Header::into_rmw_message(std::borrow::Cow::Owned(msg.header)).into_owned(),
        thumb: msg.thumb.into(),
        index: msg.index.into(),
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        header: std_msgs::msg::Header::into_rmw_message(std::borrow::Cow::Borrowed(&msg.header)).into_owned(),
        thumb: msg.thumb.as_slice().into(),
        index: msg.index.as_slice().into(),
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      header: std_msgs::msg::Header::from_rmw_message(msg.header),
      thumb: msg.thumb
          .into_iter()
          .collect(),
      index: msg.index
          .into_iter()
          .collect(),
    }
  }
}


