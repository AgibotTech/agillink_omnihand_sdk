// Copyright (c) 2025, Agibot Co., Ltd.
// OmniLogger is licensed under Mulan PSL v2.
// You can use this software according to the terms and conditions of the
// Mulan PSL v2. You may obtain a copy of Mulan PSL v2 at:
//     http://license.coscl.org.cn/MulanPSL2

/**
 * @file omnilogger.h
 * @brief Zero-allocation double-buffered async logger.
 *
 * Architecture
 * ─────────────
 *  Producers call log():
 *    1. Compile-time level check (if constexpr) — zero cost when filtered.
 *    2. Atomic runtime level check — one relaxed load, no lock.
 *    3. Lock mutex, memcpy into a fixed slot, release.
 *       Mutex hold time ≈ one memcpy (~SlotSize bytes).
 *    4. notify_one() only on empty→non-empty transition.
 *
 *  Single worker thread:
 *    1. wait_for(flush_interval) — wakes on notify or timeout.
 *    2. Swaps active ↔ drain rings in O(1) under the lock.
 *    3. Calls write_fn per slot, then flush_fn once (only if count > 0) — all outside the lock.
 *    4. Increments drained_gen_ so flush() callers can synchronise.
 *
 *  Memory: two fixed rings heap-allocated once at construction.
 *  Cache lines: active_idx, Ring metadata (head/count), and slot data are on
 *  separate 64-byte cache lines to prevent false sharing between producers
 *  and the worker thread.
 *
 * Template parameters
 * ────────────────────
 *  SlotSize       – max bytes per message (truncated if longer).
 *  RingCap        – slots per buffer half; MUST be a power of two.
 *  CompileMinLv   – messages below this value are eliminated at compile time.
 *                   Useful in release builds: set to Level::INFO to make all
 *                   debug() calls disappear with zero overhead.
 *
 * Usage — two creation paths
 * ────────────────────────────
 *  ① make_file()  —  FILE* sink; timestamps prepended by default (with_timestamp=true).
 *
 *    // stdout, INFO and above:
 *    auto log = OmniLogger<>::make_file(stdout, OmniLogger<>::Level::INFO);
 *    log->info("hello");   // → [I] [2025-11-06 17:29:45.123456] hello
 *
 *    // File sink, timestamps off (caller formats its own prefix):
 *    FILE* fp = std::fopen("/var/log/app.log", "a");
 *    auto log = OmniLogger<>::make_file(fp, OmniLogger<>::Level::INFO,
 *                                       std::chrono::milliseconds(50), false);
 *    log->warn("my own prefix: joint limit exceeded");
 *    log->flush();
 *    std::fclose(fp);
 *
 *  ② Generic constructor  —  custom write_fn; timestamps off by default (with_timestamp=false),
 *     because most custom sinks add their own formatting.
 *
 *    // ROS 2 logger sink:
 *    OmniLogger<> ros_log(
 *        [node](OmniLogger<>::Level lv, const char* msg, std::size_t len) {
 *            RCLCPP_INFO(node->get_logger(), "%.*s", static_cast<int>(len), msg);
 *        });
 *
 *    // SDK callback style — backward-compatible, logs at INFO:
 *    sdk->ShowDataDetails(true, [&ros_log](const std::string& s) { ros_log(s); });
 *
 * Formatted logging (printf-style)
 * ──────────────────────────────────
 *  logf / infof / debugf / warnf / errorf / verbosef accept a TAG and a printf
 *  format string. TAG is written as a [TAG] prefix before the message; pass
 *  nullptr or "" to omit it. Formatting is done directly into the slot buffer
 *  (zero heap allocation). GCC/Clang check format strings at compile time.
 *
 *    static constexpr const char* TAG = "CAN_RX";
 *
 *    log->infof(TAG, "frame %08x len=%d", id, len);
 *    // → [I] [2025-11-06 17:29:45.123456] [CAN_RX] frame deadbeef len=8
 *
 *    log->debugf("HandCtrl", "joint %d angle=%.2f", joint_id, angle);
 *    log->warnf(TAG,         "timeout after %d ms", elapsed_ms);
 *    log->errorf("Init",     "device not found: %s", dev_name);
 *    log->logf(Level::INFO,  nullptr, "no tag: val=%d", val);
 *
 *  // Synchronous flush (blocks until all pending messages are written):
 *  log->flush();
 */

#ifndef AGILINK_OMNILOGGER_H_
#define AGILINK_OMNILOGGER_H_

#include <array>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <string_view>
#include <thread>

namespace agilink {

template <std::size_t SlotSize     = 1024,
          std::size_t RingCap      = 2048,
          uint8_t     CompileMinLv = 0>   // 0 = Level::VERBOSE; nothing filtered at compile time
class OmniLogger {
 public:
  enum class Level : uint8_t {
    VERBOSE   = 0,  // High-frequency / per-frame tracing; filter out in normal operation
    DEBUG     = 1,  // Development and diagnostic information
    INFO      = 2,  // Normal operational events
    WARN      = 3,  // Recoverable anomalies or boundary conditions
    ERROR     = 4,  // Errors that need attention but may not terminate the program
    MAX_LEVEL = 5,  // Sentinel: count of real levels; use as min_level to silence all output
  };

  using WriteFn = std::function<void(Level, const char*, std::size_t)>;
  using FlushFn = std::function<void()>;

  static_assert((RingCap & (RingCap - 1)) == 0, "RingCap must be a power of two");
  static constexpr std::size_t kMask = RingCap - 1;

  /**
   * @param write_fn        Called by the worker for each message. Must not throw.
   * @param flush_fn        Called once per drain cycle, only when count > 0. May be nullptr.
   * @param min_level       Runtime minimum level; overrides CompileMinLv if higher.
   * @param flush_interval  Maximum output latency under sustained load.
   * @param with_timestamp  Prepend [YYYY-mm-dd HH:MM:SS.uuuuuu] to every message (local time).
   *                        Defaults to false here — custom write_fn usually handles its own
   *                        formatting (e.g. RCLCPP already adds timestamps).
   *                        make_file() defaults to true.
   */
  explicit OmniLogger(WriteFn write_fn,
                      FlushFn flush_fn = nullptr,
                      Level min_level = Level::DEBUG,
                      std::chrono::milliseconds flush_interval = std::chrono::milliseconds(50),
                      bool with_timestamp = false)
      : write_fn_(std::move(write_fn)),
        flush_fn_(std::move(flush_fn)),
        min_level_(static_cast<uint8_t>(min_level)),
        flush_interval_(flush_interval),
        with_timestamp_(with_timestamp),
        rings_(std::make_unique<Rings>()),
        worker_(&OmniLogger::run, this)
  {}

  ~OmniLogger() {
    stop();
  }

  OmniLogger(const OmniLogger&) = delete;
  OmniLogger& operator=(const OmniLogger&) = delete;
  OmniLogger(OmniLogger&&) = delete;
  OmniLogger& operator=(OmniLogger&&) = delete;

  // ── Logging ────────────────────────────────────────────────────────────────

  void log(Level level, std::string_view msg) noexcept {
    // ① Compile-time filter: dead code eliminated by the optimiser.
    if constexpr (CompileMinLv > 0) {
      if (static_cast<uint8_t>(level) < CompileMinLv) {
        return;
      }
    }
    // ② Runtime filter: one relaxed atomic load, no lock.
    if (static_cast<uint8_t>(level) < min_level_.load(std::memory_order_relaxed)) {
      return;
    }

    bool was_empty;
    static constexpr const char* kLvStr[] = {"[V] ", "[D] ", "[I] ", "[W] ", "[E] ", "[?] "};
    const char* lv = kLvStr[std::min(static_cast<uint8_t>(level), uint8_t{5})];
    char tmp[SlotSize];
    if (with_timestamp_) {
      const auto now = std::chrono::system_clock::now();
      const auto t   = std::chrono::system_clock::to_time_t(now);
      const auto us  = std::chrono::duration_cast<std::chrono::microseconds>(
                           now.time_since_epoch()) % 1000000;
      // Cache localtime result per thread — only recalculate when the second changes.
      // Avoids repeated timezone lookups (~hundreds of ns each) under high-frequency logging.
      thread_local time_t    cached_t{-1};
      thread_local struct tm cached_tm{};
      if (t != cached_t) {
        cached_t = t;
#ifdef _WIN32
        localtime_s(&cached_tm, &t);
#else
        localtime_r(&t, &cached_tm);
#endif
      }
      const int ts_n = std::snprintf(tmp, SlotSize,
          "[%04d-%02d-%02d %02d:%02d:%02d.%06lld] %.4s",
          cached_tm.tm_year + 1900, cached_tm.tm_mon + 1, cached_tm.tm_mday,
          cached_tm.tm_hour, cached_tm.tm_min, cached_tm.tm_sec,
          static_cast<long long>(us.count()), lv);
      const std::size_t off = (ts_n > 0 && static_cast<std::size_t>(ts_n) < SlotSize)
                              ? static_cast<std::size_t>(ts_n) : 0;
      const std::size_t msg_n = std::min(msg.size(), SlotSize - 1 - off);
      std::memcpy(tmp + off, msg.data(), msg_n);
      std::lock_guard<std::mutex> lock(mutex_);
      was_empty = rings_->active().push(level, tmp, off + msg_n);
    } else {
      std::memcpy(tmp, lv, 4);
      const std::size_t msg_n = std::min(msg.size(), SlotSize - 1 - 4);
      std::memcpy(tmp + 4, msg.data(), msg_n);
      std::lock_guard<std::mutex> lock(mutex_);
      was_empty = rings_->active().push(level, tmp, 4 + msg_n);
    }
    // ③ Notify only on empty→non-empty; avoids spurious wakeups under high load.
    if (was_empty) {
      cond_.notify_one();
    }
  }

  void verbose(std::string_view msg) noexcept {
    log(Level::VERBOSE, msg);
  }

  void debug(std::string_view msg) noexcept {
    log(Level::DEBUG, msg);
  }

  void info(std::string_view msg) noexcept {
    log(Level::INFO, msg);
  }

  void warn(std::string_view msg) noexcept {
    log(Level::WARN, msg);
  }

  void error(std::string_view msg) noexcept {
    log(Level::ERROR, msg);
  }

  // ── Formatted logging (printf-style) ─────────────────────────────────────────

#if defined(__GNUC__) || defined(__clang__)
  __attribute__((format(printf, 4, 5)))
#endif
  void logf(Level level, const char* tag, const char* fmt, ...) noexcept {
    va_list ap;
    va_start(ap, fmt);
    vlogf_impl(level, tag, fmt, ap);
    va_end(ap);
  }

#if defined(__GNUC__) || defined(__clang__)
  __attribute__((format(printf, 3, 4)))
#endif
  void verbosef(const char* tag, const char* fmt, ...) noexcept {
    va_list ap;
    va_start(ap, fmt);
    vlogf_impl(Level::VERBOSE, tag, fmt, ap);
    va_end(ap);
  }

#if defined(__GNUC__) || defined(__clang__)
  __attribute__((format(printf, 3, 4)))
#endif
  void debugf(const char* tag, const char* fmt, ...) noexcept {
    va_list ap;
    va_start(ap, fmt);
    vlogf_impl(Level::DEBUG, tag, fmt, ap);
    va_end(ap);
  }

#if defined(__GNUC__) || defined(__clang__)
  __attribute__((format(printf, 3, 4)))
#endif
  void infof(const char* tag, const char* fmt, ...) noexcept {
    va_list ap;
    va_start(ap, fmt);
    vlogf_impl(Level::INFO, tag, fmt, ap);
    va_end(ap);
  }

#if defined(__GNUC__) || defined(__clang__)
  __attribute__((format(printf, 3, 4)))
#endif
  void warnf(const char* tag, const char* fmt, ...) noexcept {
    va_list ap;
    va_start(ap, fmt);
    vlogf_impl(Level::WARN, tag, fmt, ap);
    va_end(ap);
  }

#if defined(__GNUC__) || defined(__clang__)
  __attribute__((format(printf, 3, 4)))
#endif
  void errorf(const char* tag, const char* fmt, ...) noexcept {
    va_list ap;
    va_start(ap, fmt);
    vlogf_impl(Level::ERROR, tag, fmt, ap);
    va_end(ap);
  }

  // ── Multi-line helpers ────────────────────────────────────────────────────────
  // Splits `s` on '\n' and logs each non-empty line individually.
  // Useful for ToString() output that spans multiple lines.

  void log_lines(Level level, const char* tag, const char* s) noexcept {
    const char* p = s;
    while (*p) {
      const char* nl  = std::strchr(p, '\n');
      const char* end = nl ? nl : p + std::strlen(p);
      if (end > p) logf(level, tag, "%.*s", static_cast<int>(end - p), p);
      p = nl ? nl + 1 : end;
    }
  }

  void info_lines (const char* tag, const char* s) noexcept { log_lines(Level::INFO,  tag, s); }
  void warn_lines (const char* tag, const char* s) noexcept { log_lines(Level::WARN,  tag, s); }
  void error_lines(const char* tag, const char* s) noexcept { log_lines(Level::ERROR, tag, s); }
  void debug_lines(const char* tag, const char* s) noexcept { log_lines(Level::DEBUG, tag, s); }

  // Backward-compatible callback style: operator()(line) logs at INFO.
  void operator()(const std::string& line) noexcept {
    log(Level::INFO, line);
  }

  void set_min_level(Level level) noexcept {
    min_level_.store(static_cast<uint8_t>(level), std::memory_order_relaxed);
  }

  /**
   * @brief Block until all messages enqueued before this call have been
   *        written and flush_fn has been called.  Safe to call from any thread.
   *
   * Waits for two complete drain cycles (generation +2).
   * Even if a drain is mid-flight when flush() is called, the second cycle
   * guarantees the active ring at call time is fully drained.
   * Worst-case wait: 2 × flush_interval (100 ms for the default).
   */
  void flush() noexcept {
    std::unique_lock<std::mutex> lk(flush_mu_);
    const uint64_t target = drained_gen_ + 2;
    cond_.notify_one();
    flush_cv_.wait(lk, [&] {
      return drained_gen_ >= target || stopping_.load(std::memory_order_relaxed);
    });
  }

  // ── Factory ────────────────────────────────────────────────────────────────

  /**
   * @brief FILE* sink with level prefix. One fflush per drain cycle (not per line).
   *
   * @param fp              Output file (default stdout).
   * @param min_level       Minimum level to write.
   * @param flush_interval  Maximum output latency under sustained load.
   * @param with_timestamp  Prepend [YYYY-mm-dd HH:MM:SS.uuuuuu] to every line (default true).
   *                        Pass false when the caller formats its own prefix.
   *
   * Level prefix: "[D]" / "[I]" / "[W]" / "[E]".
   */
  static std::unique_ptr<OmniLogger> make_file(
      FILE* fp = stdout,
      Level min_level = Level::DEBUG,
      std::chrono::milliseconds flush_interval = std::chrono::milliseconds(50),
      bool with_timestamp = true) {
    return std::make_unique<OmniLogger>(
        [fp](Level lv, const char* msg, std::size_t len) noexcept {
          std::fwrite(msg, 1, len, fp);
          if (len == 0 || msg[len - 1] != '\n') {
            std::fputc('\n', fp);
          }
        },
        [fp]() noexcept {
          std::fflush(fp);
        },
        min_level,
        flush_interval,
        with_timestamp);
  }

 private:
  // ── Internal types ─────────────────────────────────────────────────────────

  struct Slot {
    uint16_t len{0};
    Level    level{Level::DEBUG};
    char     buf[SlotSize]{};
  };

  // Cache-line layout (64-byte lines on x86/ARM):
  //   [0..63]  : head + count (metadata, accessed by producers AND worker)
  //   [64..]   : slots[] (bulk data, accessed only in sequence)
  // Separating metadata from data prevents slot writes from invalidating
  // the metadata cache line for producers waiting to push.
  struct Ring {
    alignas(64) std::size_t head{0};
    std::size_t             count{0};
    char _meta_pad[64 - 2 * sizeof(std::size_t)]{};

    std::array<Slot, RingCap> slots{};

    // Returns true if the ring was empty before this push.
    bool push(Level lv, const char* data, std::size_t len) noexcept {
      const bool was_empty = (count == 0);
      Slot& s = slots[(head + count) & kMask];
      s.level = lv;
      s.len   = static_cast<uint16_t>(len);
      std::memcpy(s.buf, data, len);
      if (count < RingCap) {
        ++count;
      } else {
        head = (head + 1) & kMask;  // drop oldest on overflow
      }
      return was_empty;
    }

    const Slot& at(std::size_t i) const noexcept {
      return slots[(head + i) & kMask];
    }

    void clear() noexcept {
      head = 0;
      count = 0;
    }
  };

  // active_idx is written by the worker (swap) and read by producers (active()).
  // Own cache line prevents worker swaps from invalidating producers' ring view.
  struct Rings {
    alignas(64) int active_idx{0};
    char _idx_pad[64 - sizeof(int)]{};

    std::array<Ring, 2> data{};

    Ring& active() noexcept {
      return data[active_idx];
    }

    Ring& drain()  noexcept {
      return data[active_idx ^ 1];
    }

    void swap() noexcept {
      active_idx ^= 1;
      active().clear();
    }
  };

  // ── Worker ─────────────────────────────────────────────────────────────────

  void stop() noexcept {
    {
      // Hold mutex so the worker's wait_for predicate sees stopping_ = true
      // atomically with respect to the condition variable.
      std::lock_guard<std::mutex> lock(mutex_);
      stopping_.store(true, std::memory_order_relaxed);
    }
    cond_.notify_one();
    if (worker_.joinable()) {
      worker_.join();
    }
  }

  void run() noexcept {
    for (;;) {
      Ring* drain_ring;
      {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait_for(lock, flush_interval_, [this] {
          return stopping_.load(std::memory_order_relaxed) || rings_->active().count > 0;
        });
        if (stopping_.load(std::memory_order_relaxed) && rings_->active().count == 0) {
          // Nothing new to write; previous drain already called flush_fn_.
          notify_drained();
          return;
        }
        rings_->swap();
        drain_ring = &rings_->drain();  // exclusively ours until next swap
      }

      for (std::size_t i = 0; i < drain_ring->count; ++i) {
        const Slot& s = drain_ring->at(i);
        write_fn_(s.level, s.buf, s.len);
      }
      // flush_fn_ only when there was actual output; avoids spurious fflush on timeout.
      if (drain_ring->count > 0 && flush_fn_) {
        flush_fn_();
      }
      notify_drained();
    }
  }

  void notify_drained() noexcept {
    {
      std::lock_guard<std::mutex> lk(flush_mu_);
      ++drained_gen_;
    }
    flush_cv_.notify_all();
  }

  void vlogf_impl(Level level, const char* tag, const char* fmt, va_list ap) noexcept {
    // Compile-time and runtime filter before doing any formatting work.
    if constexpr (CompileMinLv > 0) {
      if (static_cast<uint8_t>(level) < CompileMinLv) {
        return;
      }
    }
    if (static_cast<uint8_t>(level) < min_level_.load(std::memory_order_relaxed)) {
      return;
    }
    char buf[SlotSize];
    std::size_t start = 0;
    if (tag && tag[0] != '\0') {
      const int off = std::snprintf(buf, SlotSize, "[%s] ", tag);
      if (off > 0 && static_cast<std::size_t>(off) < SlotSize) {
        start = static_cast<std::size_t>(off);
      }
    }
    const int n = std::vsnprintf(buf + start, SlotSize - start, fmt, ap);
    const std::size_t total = (n > 0)
        ? std::min(start + static_cast<std::size_t>(n), SlotSize - 1)
        : start;
    if (total > 0) {
      log(level, std::string_view(buf, total));
    }
  }

  // ── Members ────────────────────────────────────────────────────────────────

  WriteFn                   write_fn_;
  FlushFn                   flush_fn_;
  std::atomic<uint8_t>      min_level_;
  std::chrono::milliseconds flush_interval_;
  bool                      with_timestamp_;

  std::mutex              mutex_;
  std::condition_variable cond_;
  std::unique_ptr<Rings>  rings_;
  std::atomic<bool>       stopping_{false};  // atomic: read by flush() outside mutex_

  std::mutex              flush_mu_;
  std::condition_variable flush_cv_;
  uint64_t                drained_gen_{0};   // protected by flush_mu_

  std::thread worker_;
};

}  // namespace agilink

#endif  // AGILINK_OMNILOGGER_H_
