#if USE_DAQ217
#pragma once
#include <atomic>
#include <condition_variable>
#include <future>
#include <thread>

#include "libdaq/device/base_device.h"
#include "libdaq/device/base_device_config.h"

namespace libdaq {
namespace device {
/**
 * @brief DAQ217设备控制类
 * @see 该类继承BaseDevice实现，更多详细函数参数前往 BaseDevice 查看。
 */
class DAQ217 : public BaseDevice {
 public:
  DAQ217();
  ~DAQ217() override;
  bool UseUSBBackend() override;
  bool UseUDPBackend() override;
  bool InitializeDevice() override;
  bool ConnectedDevice() override;

  bool ConfigureADCParameters(DAQADCSampleRate sample_rate, DAQVoltage voltage) override;
  bool ConfigADCSampleRate(DAQADCSampleRate sample_rate) override;
  bool ConfigADCVoltage(const DAQVoltage &voltage) override;
  bool ConfigADCTrigger(DAQADCTriggerType type, DAQADCTriggerMode mode, DAQADCTriggerChannel channel, uint32_t level) override;
  bool ConfigADCTriggerLength(uint32_t data_length) override;
  bool ConfigADCChannel(uint8_t channel) override;
  bool StartADCCollection() override;
  bool StopADCCollection() override;

 protected:
  bool ReceivePacket(int receive_packet_length) override;
  bool ReceiveData(int timeout) override;
  bool ParseData(const unique_ptr<uint8_t> &raw_data_buffer, int raw_data_size) override;

  bool SendConnected() override;
  bool SendStartCollection() override;
  bool SendStopCollection() override;

 private:
  std::vector<uint64_t> trigger_data_length_ = {0, 0, 0, 0};
};
}  // namespace device
}  // namespace libdaq
#endif