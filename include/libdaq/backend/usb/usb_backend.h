#pragma once

#include "libdaq/backend/base_backend.h"
#include "libdaq/backend/base_backend_option.h"

extern "C" {
#include "libusb-1.0/libusb.h"
}

namespace libdaq {
namespace backend {
/**
 * @brief USB控制后端
 *
 * 该后端使用libusb完成，更多详细内容请查看[Github Repo](https://github.com/libusb/libusb)
 *
 * @todo libusb在Windows平台不支持热拔插，后续需要统一添加对热拔插的支持
 * @attention 该类文档仅供内部使用，二次开发用户无需关注该类
 */
class USBBackend : public BaseBackend {
 public:
  USBBackend() = default;
  ~USBBackend() override;
  bool Initialize(const BaseBackendOption &runtime_option) override;
  bool Open() override;
  bool ReadData(unsigned char *data, int read_size, int *actual_size,
				int timeout) override;
  bool WriteData(unsigned char *data, int write_size, int *actual_size,
				 int timeout) override;
  bool ReadData(uint8_t endpoint_index, unsigned char *data, int read_size,
				int *actual_size, int timeout) override;

 private:
  libusb_context *context_ = nullptr;
  libusb_device_handle *handle_ = nullptr;
  USBOption usb_option_;
};
}  // namespace backend
}  // namespace libdaq
