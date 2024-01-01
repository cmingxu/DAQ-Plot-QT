#if USE_DAQ132M
#pragma once

#include "libdaq/device/base_device.h"

namespace libdaq {
namespace device {
class DAQ132M : public BaseDevice {
 public:
  DAQ132M();
  ~DAQ132M() override;

  bool UseModbusRTUBackend(const std::string &uart_name) override;

  bool InitializeDevice() override;
  bool ConnectedDevice() override;

  bool StartADCCollection() override;
  bool StopADCCollection() override;

 protected:
  void ReceiveThreadFunction() override;
};
}
}
#endif