std_msgs/Header header

# ePosi = 0,                    // 位置控制
# eServo = 1,                   // 伺服模式（仅 O10 支持，O12 不支持）
# eVelo = 2,                    // 速度控制
# eTorque = 3,                  // 力矩控制（不支持，使用混合模式）
# ePosiTorque = 4,              // 位置-力矩混合控制
# eVeloTorque = 5,              // 速度-力矩混合控制
# ePosiVeloTorque = 6,          // 位置-速度-力矩混合控制
# eUnknown = 10                 // 未知模式

int8[] modes
