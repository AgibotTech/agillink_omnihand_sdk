uint8 online_state         # 1~传感器在线;0~传感器不在线
uint16[] channel_value     # 各通道值
uint16 normal_force        # 法向力:(0-3000,0.1N)
uint16 tangent_force       # 切向力
uint16 tangent_force_angle # 切向力角度，指尖向上为0度，顺时针旋转:(0~359)
uint8[] capa_approach     # 自电容接近
