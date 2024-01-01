#if USE_DAQ329
#pragma once

#define USE_DEBUG 1

#include "libdaq/device/base_device.h"

namespace libdaq {
namespace device {
using std::vector;
/**
 * @brief DAQ329设备控制类
 *
 * - 数据包特性：命令回传包固定32个字节，数据回传包固定500个字节。
 * -
 * 由于数据包特性，DAQ329将不再统一处理数据接收环节，改为每一次发送命令后单独处理接收命令回传包。
 * -
 * 当发送开始采集命令后，打开数据接收线程接收数据，此时不再允许发送除停止采集命令外的所有命令。
 */
class DAQ329 : public BaseDevice {
 public:
  DAQ329();
  ~DAQ329() override;

  const std::string& GetHardwareVersion() override;
  const std::string& GetBootLoaderVersion() override;
  const std::string& GetSoftwareVersion() override;

  bool UseSerialBackend(const std::string& uart_name) override;
  bool UseTCPClientBackend(const std::string &ip_address) override;

  bool InitializeDevice() override;
  bool ConnectedDevice() override;

  bool ConfigureADCParameters(DAQADCSampleRate sample_rate, DAQVoltage voltage) override;
  bool ConfigADCSampleRate(DAQADCSampleRate sample_rate) override;
  bool ConfigADCVoltage(const DAQVoltage& voltage) override;
  bool StartADCCollection() override;
  bool StopADCCollection() override;

  bool ConfigDAC(const DAQDACChannel& channel, const DAQDACMode& mode, const std::vector<double>& data) override;

  bool ConfigGPO(int gpo_channel) override;
  std::vector<uint8_t> ReadGPI() override;

  bool ConfigPWM(DAQPWMChannel channel, uint32_t frequency, uint16_t duty_cycle) override;
  bool StartPWM(DAQPWMChannel channel) override;
  bool StopPWM(DAQPWMChannel channel) override;

  bool ConfigCounter(DAQCounterChannel channel, DAQCounterMode mode, DAQCounterTriggerType type) override;
  bool StartCounter(DAQCounterChannel channel) override;
  bool StopCounter(DAQCounterChannel channel) override;
  bool ClearCounter(DAQCounterChannel channel) override;
  std::vector<uint32_t> ReadCounter() override;

#if USE_DEBUG
  bool ADCDataIsReady(uint32_t data_num);
#endif
 protected:
  bool ReceiveData(int timeout) override;
  bool ParseData(const unique_ptr<uint8_t>& raw_data_buffer,
                 int raw_data_size) override;
  void ReceiveThreadFunction() override;

  bool SendCommand(uint16_t command, const std::vector<uint8_t>& data) override;
  bool SendStopCollection() override;

  bool GetSoftwareVersionFunction(uint8_t*) override;
  bool GetHardwareVersionFunction(uint8_t*) override;
  bool GetBootLoaderFunction(uint8_t*) override;
  bool GetADCFunction(uint8_t*) override;
  bool GetGPIFunction(uint8_t*) override;
  bool GetCounterFunction(uint8_t*) override;

 private:
  std::vector<uint8_t> temp_data_;
};
}  // namespace device
}  // namespace libdaq
#endif