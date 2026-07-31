#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};



// Corresponds to omnihand_pro_2025_node_msgs__msg__TactileSensorData

// This struct is not documented.
#[allow(missing_docs)]

#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct TactileSensorData {
    /// 1=sensor online; 0=sensor offline
    pub online_state: u8,

    /// 6-channel 24-bit value
    pub channel_value: Vec<u32>,

    /// normal force (0.1 N, 0 ~ 3000)
    pub normal_force: u16,

    /// tangent force (0.1 N, 0 ~ 3000)
    pub tangent_force: u16,

    /// tangent force angle, fingertip-up = 0°, clockwise (0~359)
    pub tangent_force_angle: u16,

    /// 4 self-capacitance proximity values
    pub capa_approach: Vec<u8>,

}



impl Default for TactileSensorData {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::msg::rmw::TactileSensorData::default())
  }
}

impl rosidl_runtime_rs::Message for TactileSensorData {
  type RmwMsg = super::msg::rmw::TactileSensorData;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        online_state: msg.online_state,
        channel_value: msg.channel_value.into(),
        normal_force: msg.normal_force,
        tangent_force: msg.tangent_force,
        tangent_force_angle: msg.tangent_force_angle,
        capa_approach: msg.capa_approach.into(),
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
      online_state: msg.online_state,
        channel_value: msg.channel_value.as_slice().into(),
      normal_force: msg.normal_force,
      tangent_force: msg.tangent_force,
      tangent_force_angle: msg.tangent_force_angle,
        capa_approach: msg.capa_approach.as_slice().into(),
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      online_state: msg.online_state,
      channel_value: msg.channel_value
          .into_iter()
          .collect(),
      normal_force: msg.normal_force,
      tangent_force: msg.tangent_force,
      tangent_force_angle: msg.tangent_force_angle,
      capa_approach: msg.capa_approach
          .into_iter()
          .collect(),
    }
  }
}


// Corresponds to omnihand_pro_2025_node_msgs__msg__TactileSensor
/// OmniHand Pro 2025 (O12) 3D tactile: one TactileSensorData per finger (thumb … palm).

#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct TactileSensor {

    // This member is not documented.
    #[allow(missing_docs)]
    pub header: std_msgs::msg::Header,


    // This member is not documented.
    #[allow(missing_docs)]
    pub thumb: super::msg::TactileSensorData,


    // This member is not documented.
    #[allow(missing_docs)]
    pub index: super::msg::TactileSensorData,


    // This member is not documented.
    #[allow(missing_docs)]
    pub middle: super::msg::TactileSensorData,


    // This member is not documented.
    #[allow(missing_docs)]
    pub ring: super::msg::TactileSensorData,


    // This member is not documented.
    #[allow(missing_docs)]
    pub little: super::msg::TactileSensorData,


    // This member is not documented.
    #[allow(missing_docs)]
    pub palm: super::msg::TactileSensorData,

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
        thumb: super::msg::TactileSensorData::into_rmw_message(std::borrow::Cow::Owned(msg.thumb)).into_owned(),
        index: super::msg::TactileSensorData::into_rmw_message(std::borrow::Cow::Owned(msg.index)).into_owned(),
        middle: super::msg::TactileSensorData::into_rmw_message(std::borrow::Cow::Owned(msg.middle)).into_owned(),
        ring: super::msg::TactileSensorData::into_rmw_message(std::borrow::Cow::Owned(msg.ring)).into_owned(),
        little: super::msg::TactileSensorData::into_rmw_message(std::borrow::Cow::Owned(msg.little)).into_owned(),
        palm: super::msg::TactileSensorData::into_rmw_message(std::borrow::Cow::Owned(msg.palm)).into_owned(),
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        header: std_msgs::msg::Header::into_rmw_message(std::borrow::Cow::Borrowed(&msg.header)).into_owned(),
        thumb: super::msg::TactileSensorData::into_rmw_message(std::borrow::Cow::Borrowed(&msg.thumb)).into_owned(),
        index: super::msg::TactileSensorData::into_rmw_message(std::borrow::Cow::Borrowed(&msg.index)).into_owned(),
        middle: super::msg::TactileSensorData::into_rmw_message(std::borrow::Cow::Borrowed(&msg.middle)).into_owned(),
        ring: super::msg::TactileSensorData::into_rmw_message(std::borrow::Cow::Borrowed(&msg.ring)).into_owned(),
        little: super::msg::TactileSensorData::into_rmw_message(std::borrow::Cow::Borrowed(&msg.little)).into_owned(),
        palm: super::msg::TactileSensorData::into_rmw_message(std::borrow::Cow::Borrowed(&msg.palm)).into_owned(),
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      header: std_msgs::msg::Header::from_rmw_message(msg.header),
      thumb: super::msg::TactileSensorData::from_rmw_message(msg.thumb),
      index: super::msg::TactileSensorData::from_rmw_message(msg.index),
      middle: super::msg::TactileSensorData::from_rmw_message(msg.middle),
      ring: super::msg::TactileSensorData::from_rmw_message(msg.ring),
      little: super::msg::TactileSensorData::from_rmw_message(msg.little),
      palm: super::msg::TactileSensorData::from_rmw_message(msg.palm),
    }
  }
}


