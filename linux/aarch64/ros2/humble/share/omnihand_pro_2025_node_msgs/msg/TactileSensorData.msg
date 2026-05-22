uint8 online_state         # 1=sensor online; 0=sensor offline
uint32[] channel_value     # 6-channel 24-bit value
uint16 normal_force        # normal force (0.1 N, 0 ~ 3000)
uint16 tangent_force       # tangent force (0.1 N, 0 ~ 3000)
uint16 tangent_force_angle # tangent force angle, fingertip-up = 0°, clockwise (0~359)
uint8[] capa_approach      # 4 self-capacitance proximity values
