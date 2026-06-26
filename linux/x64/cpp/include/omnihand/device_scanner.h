// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

/**
 * @file device_scanner.h
 * @brief CANFD device scanner (public API)
 * @author SDK Team
 * @date 2025
 **/

#ifndef AGILINK_OMNIHAND_DEVICE_SCANNER_H
#define AGILINK_OMNIHAND_DEVICE_SCANNER_H

#include <string>
#include <vector>

#include "omnihand/export_symbols.h"
#include "omnihand/utils.h"

namespace agilink {
namespace omnihand {

AGIBOT_EXPORT std::vector<ScannedCanfdDeviceInfo> ScanZlgCanfdDevices();
AGIBOT_EXPORT std::vector<ScannedCanfdDeviceInfo> ScanHcanDevices();
AGIBOT_EXPORT std::vector<ScannedSerialDeviceInfo> ScanSerialDevices();

}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OMNIHAND_DEVICE_SCANNER_H
