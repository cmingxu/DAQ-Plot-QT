#pragma once
#include "libdaq/backend/base_backend.h"
#include "libdaq/backend/base_backend_option.h"

#include <modbus.h>

namespace libdaq {
namespace backend {
class ModbusRTUBackend : public BaseBackend {
 public:
  ModbusRTUBackend() = default;
  ~ModbusRTUBackend() override;

  bool Initialize(const BaseBackendOption &runtime_option) override;
  bool Open() override;
  bool ReadData(uint8_t endpoint_index, unsigned char *data, int read_size, int *actual_size, int timeout) override;
  bool WriteData(uint8_t endpoint_index, unsigned char *data, int write_size, int *actual_size, int timeout) override;

 private:
  ModbusOption modbus_option_;
  modbus_t *modbus_t_ = nullptr;
};
}
}