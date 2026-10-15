#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};



// Corresponds to omnihand_3_lite_node_msgs__msg__TactileSensor
/// OmniHand 3 Lite (H3L) 16-bit 1D tactile data from six sensor regions.

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


    // This member is not documented.
    #[allow(missing_docs)]
    pub middle: Vec<u16>,


    // This member is not documented.
    #[allow(missing_docs)]
    pub ring: Vec<u16>,


    // This member is not documented.
    #[allow(missing_docs)]
    pub little: Vec<u16>,


    // This member is not documented.
    #[allow(missing_docs)]
    pub palm: Vec<u16>,

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
        middle: msg.middle.into(),
        ring: msg.ring.into(),
        little: msg.little.into(),
        palm: msg.palm.into(),
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        header: std_msgs::msg::Header::into_rmw_message(std::borrow::Cow::Borrowed(&msg.header)).into_owned(),
        thumb: msg.thumb.as_slice().into(),
        index: msg.index.as_slice().into(),
        middle: msg.middle.as_slice().into(),
        ring: msg.ring.as_slice().into(),
        little: msg.little.as_slice().into(),
        palm: msg.palm.as_slice().into(),
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
      middle: msg.middle
          .into_iter()
          .collect(),
      ring: msg.ring
          .into_iter()
          .collect(),
      little: msg.little
          .into_iter()
          .collect(),
      palm: msg.palm
          .into_iter()
          .collect(),
    }
  }
}


