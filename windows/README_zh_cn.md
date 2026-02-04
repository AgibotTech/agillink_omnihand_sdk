# OmniHand 2025 SDK v1.0.0 - Windows

鏀寔鐨勬灦鏋勶細x64

**骞冲彴璇存槑**锛氭湰鍖呬负 Windows 鐗堟湰銆?*ROS2 鎺ュ彛涓?SocketCAN** 鍦?Windows 涓婁笉鏀寔锛屼粎鍦?Linux 骞冲彴鎻愪緵銆傛湰鍖呬腑鐨?API 鏂囨。锛堝 doc/en/銆乣doc/zh_cn/锛夊凡鍘婚櫎涓?ROS2 鐩稿叧鐨勫唴瀹癸紝浠呬繚鐣?Windows 鍙敤鎺ュ彛璇存槑銆?

## 浜у搧姒傝堪

OmniHand 2025 SDK 鏀寔涓夌浜у搧鍨嬪彿锛?

**OmniHand 2025 鐏靛姩娆?(O10)**锛氱揣鍑戝瀷楂樿嚜鐢卞害浜や簰鐏靛阀鎵嬶紝鍏锋湁 10 涓富鍔?+ 6 涓鍔ㄨ嚜鐢卞害銆傞噸閲忎粎 500g锛岄噰鐢?CANFD 閫氫俊鎺ュ彛锛岄厤澶?400+ 瑙﹁鐐癸紝0.1N 闃靛垪鍒嗚鲸鐜囷紝鏈€澶ф寚灏栧姏 5N銆傞€傜敤浜庡悇绉嶄汉褰㈡満鍣ㄤ汉鍜屾満姊拌噦銆傚叾绱у噾杞婚噺鍖栫殑璁捐鍜屼赴瀵岀殑瑙﹁浜や簰鑳藉姏锛屼娇鍏跺湪浜や簰鏈嶅姟銆佺爺绌舵暀鑲层€佽交閲忎綔涓氱瓑棰嗗煙鍏锋湁閲嶈浠峰€笺€?

![](doc/pic/hand_o10.jpg)

**OmniHand Pro 2025 涓撲笟娆?(O12)**锛?2 鑷敱搴︿笓涓氱伒宸ф墜锛屽叿鏈夌簿纭搷浣滃拰鐏垫椿鎺у埗鑳藉姏銆傞厤澶囪Е瑙変紶鎰熷櫒鍜屽绉嶆帶鍒舵ā寮忥紙浣嶇疆鎺у埗銆佸姏鐭╂帶鍒躲€佹贩鍚堟帶鍒讹級锛岄€傜敤浜庣爺绌舵暀鑲层€佸ū涔愬晢涓氭紨鍑恒€佸睍瑙堝紩瀵笺€佸伐涓氬満鏅瓑澶氱搴旂敤銆?

![](doc/pic/hand_o12.jpg)

**OmniHand Dex UMI (O10 UMI)**锛氫娇鐢?UMI 鍗忚鐨勫彧璇荤伒宸ф墜锛屾敮鎸佸懆鏈熸€х殑浣嶇疆鍜岃Е瑙変紶鎰熷櫒鏁版嵁涓婃姤銆?

## 鐏靛阀鎵嬬數鏈虹储寮?

**OmniHand 2025 鐏靛姩娆?(O10)**锛氬叿鏈?10 涓嚜鐢卞害锛岀储寮曚粠 1 鍒?10銆傚搴旂殑鎺у埗鐢垫満濡備笅鍥炬墍绀猴細

![](doc/pic/hand_o10_motors.jpg)

**OmniHand Pro 2025 涓撲笟娆?(O12)**锛氬叿鏈?12 涓嚜鐢卞害锛岀储寮曚粠 1 鍒?12銆傚搴旂殑鎺у埗鐢垫満濡備笅鍥炬墍绀猴細

![](doc/pic/hand_o12_motors.jpg)

## 绯荤粺瑕佹眰

### 纭欢瑕佹眰

鏀寔浠ヤ笅閫氫俊鎺ュ彛锛?

- **CANFD (USB 閫傞厤鍣? - 鎺ㄨ崘**锛歓LG USBCANFD 绯诲垪锛圲SBCANFD-100U-mini/USBCANFD-100U/USBCANFD-200U锛?
  - 鉁?**闆堕厤缃?*锛歋DK 鍖呭惈搴撴枃浠讹紝寮€绠卞嵆鐢?
  - 鉁?**鏃犻渶绠＄悊鍛樻潈闄?*锛氱敤鎴风┖闂村簱
  - 鉁?**绠€鍗?API**锛歚OmniHand2025.create_hand_by_zlgcan(...) / OmniHandPro2025.create_hand_by_zlgcan(...) / OmniHandDexUMI.create_hand_by_zlgcan(...)
- **RS485锛堜粎 OmniHand 2025锛?*锛氫覆鍙ｉ€氫俊
- **Windows 涓嶆敮鎸?*锛歊OS2 鎺ュ彛涓?SocketCAN 浠呭湪 Linux 鎻愪緵锛涙湰鍖呭唴 API 鏂囨。宸插幓闄?ROS2 鐩稿叧鍐呭銆?

### 杞欢瑕佹眰

- **鎿嶄綔绯荤粺**锛歐indows 10/11 (x64)
- **缂栬瘧鍣?*锛歁SVC 2019+ 鎴栧吋瀹圭増鏈?
- **鏋勫缓宸ュ叿**锛欳Make 3.24 鎴栨洿楂樼増鏈紙鐢ㄤ簬鏋勫缓绀轰緥锛?
- **Python**锛?.10 鎴栨洿楂樼増鏈紙鐢ㄤ簬 Python SDK锛?

## 蹇€熷畨瑁?

浠ョ鐞嗗憳韬唤杩愯锛?
```
install.bat
```

鎴栨寚瀹氳矾寰勶細
```
install.bat "D:\omnihand2025"
```

## 鍗歌浇

浠ョ鐞嗗憳韬唤杩愯锛?
```
uninstall.bat
```

## 鐩綍缁撴瀯

```
windows/
鈹溾攢鈹€ cpp/
鈹?  鈹溾攢鈹€ share/
鈹?  鈹?  鈹斺攢鈹€ cmake/
鈹?  鈹?      鈹斺攢鈹€ omnihand/        # CMake 閰嶇疆
鈹?  鈹溾攢鈹€ include/omnihand/         # 澶存枃浠?
鈹?  鈹溾攢鈹€ lib/                      # C++ 搴?
鈹?  鈹溾攢鈹€ demo/                      # C++ 绀轰緥婧愮爜锛堜笉瀹夎锛?
鈹?  鈹?  鈹溾攢鈹€ omnihand_2025/
鈹?  鈹?  鈹溾攢鈹€ omnihand_pro_2025/
鈹?  鈹?  鈹斺攢鈹€ omnihand_dex_umi/
鈹?  鈹溾攢鈹€ test/                      # C++ 娴嬭瘯婧愮爜锛堜笉瀹夎锛?
鈹?  鈹斺攢鈹€ bin/omnihand/
鈹?      鈹溾攢鈹€ demo/                  # 绀轰緥鍙墽琛屾枃浠?
鈹?      鈹斺攢鈹€ test/                   # 娴嬭瘯鍙墽琛屾枃浠?
鈹溾攢鈹€ python/
鈹?  鈹溾攢鈹€ *.whl                     # Python wheel
鈹?  鈹溾攢鈹€ demo/                     # Python 绀轰緥锛堜笉瀹夎锛?
鈹?  鈹?  鈹溾攢鈹€ omnihand_2025/
鈹?  鈹?  鈹溾攢鈹€ omnihand_pro_2025/
鈹?  鈹?  鈹斺攢鈹€ omnihand_dex_umi/
鈹?  鈹斺攢鈹€ test/                      # Python 娴嬭瘯锛堜笉瀹夎锛?
鈹溾攢鈹€ doc/                          # 鏂囨。
鈹溾攢鈹€ install.bat                   # 瀹夎鑴氭湰
鈹溾攢鈹€ uninstall.bat                 # 鍗歌浇鑴氭湰
鈹溾攢鈹€ README.md                     # [English Documentation](README.md)
鈹斺攢鈹€ README_zh_cn.md               # 鏈枃妗ｏ紙涓枃锛?
```

## C++ 浣跨敤

```cmake
set(OMNIHAND_ROOT "C:/Program Files/omnihand2025")
list(APPEND CMAKE_MODULE_PATH "${OMNIHAND_ROOT}/share/cmake/omnihand")
find_package(omnihand REQUIRED)
target_link_libraries(your_target omnihand)  # 缁熶竴搴撴敮鎸?OmniHand 2025 (O10)銆丱mniHand Pro 2025 (O12) 鍜?OmniHand Dex UMI (O10 UMI)
```

C++ 绀轰緥浠ｇ爜锛堟帹鑽愶細ZLG USB CANFD - 闆堕厤缃級锛?

```cpp
#include "omnihand/omnihand_2025.h"  // 鐢ㄤ簬 OmniHand 2025 (O10)
// #include "omnihand/omnihand_pro_2025.h"  // 鐢ㄤ簬 OmniHand Pro 2025 (O12)

int main() {
    // OmniHand 2025 (O10, 10 DOF) - 鎺ㄨ崘锛歓LG USB CANFD
    //   hand_type    = EHandType::eLeft      // 宸︽墜鎴栧彸鎵?
    //   device_id    = 1                     // 鍐欏叆鐏靛阀鎵嬪浐浠剁殑璁惧 ID
    //   canfd_id     = 0                     // USB CANFD 閫傞厤鍣ㄧ储寮?
    //   channel_id   = 0                     // 璇ラ€傞厤鍣ㄤ笂鐨?CAN 閫氶亾绱㈠紩
    auto hand = OmniHand2025::createHandByZlgcan(EHandType::eLeft, 1, 0, 0);

    if (!hand || !hand->Init()) {
        std::cerr << "鍒濆鍖栧け璐? << std::endl;
        return -1;
    }

    // 璁剧疆鎵€鏈夊叧鑺傝搴︼紙鎺ㄨ崘锛氭眰瑙ｅ櫒鑷姩杞崲涓虹數鏈轰綅缃級
    std::vector<double> angles(10, 0.0);  // 10 涓叧鑺傦紝鍏ㄩ儴璁剧疆涓?0 寮у害
    hand->SetAllActiveJointAngles(angles);

    return 0;
}
```

鏇村绀轰緥锛岃鍙傞槄 [cpp/demo/](cpp/demo/) 鐩綍锛屽寘鍚骇鍝佺壒瀹氱殑瀛愮洰褰曘€?

璇︾粏鐨?C++ API 璇存槑锛岃鍙傞槄 [doc/zh_cn/API_CPP.md](doc/zh_cn/API_CPP.md) - 鍖呭惈浜у搧鐗瑰畾 API 閾炬帴鐨勭储寮曢〉锛?
- [OmniHand 2025 (O10) C++ API](doc/zh_cn/API_CPP_O10.md)
- [OmniHand Pro 2025 (O12) C++ API](doc/zh_cn/API_CPP_O12.md)
- [OmniHand Dex UMI (O10 UMI) C++ API](doc/zh_cn/API_CPP_O10_UMI.md)

## Python 浣跨敤

```python
# 鎺ㄨ崘锛歓LG USB CANFD锛堥浂閰嶇疆锛?
from omnihand import OmniHand2025, OmniHandPro2025, EHandType

# OmniHand 2025 (10 DOF)
hand_o10 = OmniHand2025.create_hand_by_zlgcan(
    hand_type=EHandType.RIGHT,
    hand_device_id=1,
    canfd_device_id=0,
    canfd_channel_id=0
)

if not hand_o10.init():
    print("鍒濆鍖栧け璐?)
    exit(1)

# OmniHand Pro 2025 (12 DOF)
hand_o12 = OmniHandPro2025.create_hand_by_zlgcan(
    hand_type=EHandType.LEFT,
    hand_device_id=1,
    canfd_device_id=0,
    canfd_channel_id=0
)
```

鏇村绀轰緥锛岃鍙傞槄 [python/demo/README.md](python/demo/README.md)銆?

璇︾粏鐨?Python API 璇存槑锛岃鍙傞槄 [doc/zh_cn/API_PYTHON.md](doc/zh_cn/API_PYTHON.md) - 鍖呭惈浜у搧鐗瑰畾 API 閾炬帴鐨勭储寮曢〉锛?
- [OmniHand 2025 (O10) Python API](doc/zh_cn/API_PYTHON_O10.md)
- [OmniHand Pro 2025 (O12) Python API](doc/zh_cn/API_PYTHON_O12.md)
- [OmniHand Dex UMI (O10 UMI) Python API](doc/zh_cn/API_PYTHON_UMI.md)

## 璁稿彲璇?

Mulan PSL v2 - Copyright (c) 2025, Agibot Co., Ltd.