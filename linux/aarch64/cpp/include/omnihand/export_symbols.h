// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

/**
 * @file export_symbols.h
 * @brief 导出符号宏定义
 * @author hanjun
 * @date 25-8-1
 **/

#ifndef AGILINK_EXPORT_SYMBOLS_H
#define AGILINK_EXPORT_SYMBOLS_H

#if defined(_WIN32) || defined(_WIN64)
  #ifdef BUILDING_DLL
    #define AGIBOT_EXPORT __declspec(dllexport)
  #else
    #define AGIBOT_EXPORT __declspec(dllimport)
  #endif
#else
  #define AGIBOT_EXPORT __attribute__((visibility("default")))
#endif

// ZLG CANFD over TCP (WiFi/Ethernet): supported on Windows and Linux x64, not on Linux aarch64/arm64
#if defined(_WIN32) || (defined(__linux__) && (defined(__x86_64__) || defined(__amd64__)))
  #define OMNIHAND_ZLG_TCP_SUPPORTED 1
#else
  #define OMNIHAND_ZLG_TCP_SUPPORTED 0
#endif

#endif  // AGILINK_EXPORT_SYMBOLS_H