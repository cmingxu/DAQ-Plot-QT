#pragma once

#include <iostream>

#include "libdaq/backend/base_backend_option.h"
#include "libdaq/log/log_config.h"
#define BASE_BACKEND_PRINT_UNSUPPORT DAQ_LIB_LOGGER_WARN("BaseBackend don't support this API.")

namespace libdaq {
namespace backend {
/**
 * @brief 后端控制基类，开发者无需关注
 *
 * 后端控制基类的函数大致分为初始化，打开，读取，写入4个部分。添加任何后端都需要重写这四个函数。
 *
 * @attention 该类文档仅供内部使用，二次开发用户无需关注该类
 * @return 默认返回值均为false或者0
 */
class BaseBackend {
 public:
  BaseBackend() = default;
  virtual ~BaseBackend() = default;
  virtual bool Initialize(const BaseBackendOption &runtime_option) {
    BASE_BACKEND_PRINT_UNSUPPORT
    return false;
  }

  virtual bool Open() {
    BASE_BACKEND_PRINT_UNSUPPORT
    return false;
  }

  virtual bool ReadData(unsigned char *data, int read_size, int *actual_size, int timeout = 100) {
    BASE_BACKEND_PRINT_UNSUPPORT
    return false;
  }
  virtual bool WriteData(unsigned char *data, int write_size, int *actual_size, int timeout = 100) {
    BASE_BACKEND_PRINT_UNSUPPORT
    return false;
  }

  virtual size_t GetAvailableDataSize() {
    BASE_BACKEND_PRINT_UNSUPPORT
    return 0;
  }
  virtual bool ReadData(uint8_t endpoint_index, unsigned char *data, int read_size, int *actual_size, int timeout = 100) {
    BASE_BACKEND_PRINT_UNSUPPORT
    return false;
  }
  virtual bool WriteData(uint8_t endpoint_index, unsigned char *data, int read_size, int *actual_size, int timeout = 100) {
    BASE_BACKEND_PRINT_UNSUPPORT
    return false;
  }
};
}  // namespace backend
}  // namespace libdaq
