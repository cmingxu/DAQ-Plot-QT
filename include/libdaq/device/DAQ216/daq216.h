#if USE_DAQ216
#pragma once
#include <atomic>
#include <condition_variable>
#include <future>
#include <thread>

#include "libdaq/device/base_device.h"

namespace libdaq {
namespace device {
/**
 * @brief DAQ216设备控制类
 * @see 该类继承BaseDevice实现，更多详细函数参数前往 BaseDevice 查看。
 */
class DAQ216 : public BaseDevice {
 public:
  DAQ216();
  ~DAQ216();
  bool ConnectedDevice() override;

 protected:
  std::string deveice_name = "DAQ216";

 private:
  bool ReceivePacket(int read_length, bool need_enqueue = true);
};
}  // namespace device
}  // namespace libdaq
#endif