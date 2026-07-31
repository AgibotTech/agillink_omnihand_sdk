from typing import Any, Literal

from pydantic import BaseModel, ConfigDict, Field


class VersionSchema(BaseModel):
    model_config = ConfigDict(from_attributes=True)

    major: int
    minor: int
    patch: int
    res: int


class CommuParamsSchema(BaseModel):
    model_config = ConfigDict(from_attributes=True)

    bitrate: int
    sample_point: int
    dbitrate: int
    dsample_point: int


class DeviceInfoSchema(BaseModel):
    model_config = ConfigDict(from_attributes=True)

    hand_device_id: int
    commu_params: CommuParamsSchema


class VendorInfoSchema(BaseModel):
    model_config = ConfigDict(from_attributes=True)

    product_model: str
    product_seq_num: str
    hardware_version: VersionSchema
    software_version: VersionSchema
    voltage: int
    dof: int


class JointMotorErrorReportSchema(BaseModel):
    model_config = ConfigDict(from_attributes=True)

    stalled: bool
    overheat: bool
    over_current: bool
    motor_except: bool
    commu_except: bool


class MixCtrlSchema(BaseModel):
    model_config = ConfigDict(from_attributes=True)

    joint_index: int
    ctrl_mode: int
    tgt_posi: int
    tgt_velo: int
    tgt_torque: int


class TactileSensorDataSchema(BaseModel):
    model_config = ConfigDict(from_attributes=True)

    sensor_id: int
    data: list[int]


class TactileSensor3DDataSchema(BaseModel):
    model_config = ConfigDict(from_attributes=True)

    online_state: int
    channel_values: list[int]
    normal_force: int
    tangent_force: int
    tangent_force_angle: int
    capacitive_approach: list[int]


HandType = Literal[
    "omnihand_2025",
    "omnihand_pro_2025",
    "omnihand_dex_umi",
    "omnihand_3_lite",
    "omnihand_3_ultra_m",
]

HandSide = Literal["left", "right"]

ConnMethod = Literal[
    "zlgcan",
    "hcan",
    "socketcan",
    "rs485",
    "usb",
    "tj",
    "zlgcan_tcp",
]


class HandCreateRequest(BaseModel):
    hand_type: HandType = Field(description="手部型号")
    hand_side: HandSide = Field(default="left", description="手部侧别，默认为左手")
    conn_method: ConnMethod = Field(description="连接方式")
    conn_config: dict[str, Any] = Field(default_factory=dict, description="连接参数，取决于 conn_method")


class HandCreateResponse(BaseModel):
    hand_id: int = Field(description="后端注册的唯一手部标识")
    already_created: bool = Field(description="该 hand_id 是否已存在（幂等创建）")


class HandSummary(BaseModel):
    hand_id: int
    hand_type: str
    conn_method: str


class HandDetail(BaseModel):
    hand_type: str
    conn_method: str
    device_info: DeviceInfoSchema | None = None
    vendor_info: VendorInfoSchema | None = None


class ErrorResponse(BaseModel):
    error: str


class MethodCallResponse(BaseModel):
    result: Any = Field(description="方法返回结果，类型取决于具体方法")


class MethodInfo(BaseModel):
    category: str
    name: str
    params: dict[str, str] = Field(default_factory=dict)
    returns: dict[str, str] = Field(default_factory=dict)
    description: str = ""


class MethodListResponse(BaseModel):
    product_type: str
    methods: list[MethodInfo]


class WsMethodCallRequest(BaseModel):
    type: Literal["call"]
    request_id: str | None = None
    hand_id: int
    method: str
    params: dict[str, Any] = Field(default_factory=dict)


class WsSubscriptionRequest(BaseModel):
    type: Literal["subscribe", "unsubscribe"]
    request_id: str | None = None
    hand_id: int
    streams: list[str] = Field(default_factory=list)


class WsResultMessage(BaseModel):
    type: Literal["result"] = "result"
    request_id: str | None = None
    ok: bool
    result: Any = None
    error: str | None = None


class WsEventMessage(BaseModel):
    type: Literal["event"] = "event"
    stream: str
    hand_id: int
    data: Any
