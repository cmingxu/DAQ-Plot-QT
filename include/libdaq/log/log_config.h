#pragma once

// clang-format off
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LOGGER_TRACE
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_sinks.h>
// clang-format on

namespace lockzhiner {
#define DAQ_LIB_SPD_LOGGER_NAME "libdaq"
#define DAQ_LIB_LOGGER_INFO(...)                            \
  {                                                         \
    auto get_logger = spdlog::get(DAQ_LIB_SPD_LOGGER_NAME); \
    SPDLOG_LOGGER_INFO(get_logger, __VA_ARGS__);            \
  }
#define DAQ_LIB_LOGGER_DEBUG(...)                           \
  {                                                         \
    auto get_logger = spdlog::get(DAQ_LIB_SPD_LOGGER_NAME); \
    SPDLOG_LOGGER_DEBUG(get_logger, __VA_ARGS__);           \
  }
#define DAQ_LIB_LOGGER_WARN(...)                            \
  {                                                         \
    auto get_logger = spdlog::get(DAQ_LIB_SPD_LOGGER_NAME); \
    SPDLOG_LOGGER_WARN(get_logger, __VA_ARGS__);            \
  }
#define DAQ_LIB_LOGGER_ERROR(...)                           \
  {                                                         \
    auto get_logger = spdlog::get(DAQ_LIB_SPD_LOGGER_NAME); \
    SPDLOG_LOGGER_ERROR(get_logger, __VA_ARGS__);           \
  }
#define DAQ_LIB_LOGGER_CRITICAL(...)                        \
  {                                                         \
    auto get_logger = spdlog::get(DAQ_LIB_SPD_LOGGER_NAME); \
    SPDLOG_LOGGER_CRITICAL(get_logger, __VA_ARGS__);        \
  }

}  // namespace lockzhiner
