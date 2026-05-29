// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

#ifndef AGILINK_OMNIHAND_UTILS_H
#define AGILINK_OMNIHAND_UTILS_H

#include <functional>
#include <string>

namespace agilink {
namespace omnihand {

/** Optional sink for ShowDataDetails traffic; when empty, SDK logs to stdout. */
using DataDetailsLogCallback = std::function<void(const std::string& line)>;

}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OMNIHAND_UTILS_H
