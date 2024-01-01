#pragma once

// clang-format off
#include <serial/serial.h>
#include <iostream>
#include "libdaq/backend/base_backend.h"
#include "libdaq/backend/base_backend_option.h"
// clang-format on

namespace libdaq {
namespace backend {
/**
 * @brief 串口控制后端
 *
 * 该后端使用serial完成，详细信息请前往[Github
 * Repo](https://github.com/wjwwood/serial)查看
 *
 */
class SerialBackend : public BaseBackend {
 public:
  SerialBackend() = default;
  ~SerialBackend() override;
  bool Initialize(const BaseBackendOption &runtime_option) override;
  bool Open() override;
  bool ReadData(unsigned char *data, int read_size, int *actual_size, int timeout) override;
  bool WriteData(unsigned char *data, int write_size, int *actual_size, int timeout) override;

  // Only for SerialBackend
  size_t GetAvailableDataSize() override {
    if (serial_ == nullptr) {
      return 0;
    }
    return serial_->available();
  }

  static  std::vector<std::string> GetSerialPortList();

 private:
  serial::Serial *serial_ = nullptr;
  SerialOption serial_option_;
};
}  // namespace backend
}  // namespace libdaq
