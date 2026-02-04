// Copyright (c) 2025, Agibot Co., Ltd.
// OmniHand 2025 SDK is licensed under Mulan PSL v2.

/**
 * @file export_symbols.h
 * @brief 导出符号宏定义
 * @author hanjun
 * @date 25-8-1
 **/

#ifndef EXPORT_SYMBOLS_H
#define EXPORT_SYMBOLS_H

#if defined(_WIN32) || defined(_WIN64)
  #ifdef BUILDING_DLL
    #define AGIBOT_EXPORT __declspec(dllexport)
  #else
    #define AGIBOT_EXPORT __declspec(dllimport)
  #endif
#else
  #define AGIBOT_EXPORT __attribute__((visibility("default")))
#endif

#endif  // EXPORT_SYMBOLS_H