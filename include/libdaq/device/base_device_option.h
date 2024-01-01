#pragma once

#include <string>
#include <vector>

#include "libdaq/device/base_device_config.h"

namespace libdaq {
namespace device {
struct BaseDeviceOption {
  std::string name_ = "BaseDevice";        ///< 底层设备类型
  std::string hardware_version_ = "0.0.0";  ///< 底层硬件版本
  std::string boot_loader_version_ = "0.0.0";       ///< 底层BootLoader版本
  std::string software_version_ = "0.0.0";  ///< 底层软件版本
};

struct BasePacketOption {
  uint8_t device_address_ = 0x01;             ///< 设备默认地址
  uint32_t send_packet_header_ = 0x55AA524C;  ///< 发送数据包包头
  uint32_t send_packet_tail_ = 0xA55ACCBB;    ///< 发送数据包包尾
  std::vector<uint8_t> raw_data_header_ = {
	  0x5A,
	  0x4A,
	  0x55,
	  0xAA,
  };  ///< 返回包包头
  std::vector<uint8_t> raw_data_tail_ = {
	  0xDD,
	  0xEE,
	  0xA5,
	  0x5A,
  };                            ///< 返回包包尾
  int raw_data_size_ = 0;  ///< 返回包数据长度大小
};

struct BaseADCOption {
  uint8_t legal_channel_size_ = 0;  ///< 合法的设备ADC通道数
  std::vector<DAQADCSampleRate> legal_sample_rate_ = {
	  DAQADCSampleRate::SampleRateError,
  };  ///< 合法的采样率
  std::vector<DAQVoltage> legal_voltage_ = {
	  DAQVoltage::VoltageError,
  };  ///< 合法的电压量程
  DAQADCTriggerType current_trigger_type_ = DAQADCTriggerType::Normal;              ///< 当前触发方式
  uint32_t current_trigger_data_length_ = 0;  ///< 当前触发数据长度
  uint32_t receive_data_length_max_ = 0;  ///< 触发接收数据最大长度
  uint32_t receive_data_length_min_ = 0;  ///< 触发接收数据最小长度
  uint32_t trigger_timeout_max_ = 0;  ///< 触发接收数据最大超时时间
  uint32_t trigger_timeout_min_ = 0;  ///< 触发接收数据最小超时时间
  DAQADCSampleRate current_sample_rate_ = DAQADCSampleRate::SampleRateError;                   ///< 当前采样率
  DAQVoltage current_voltage_ = DAQVoltage::VoltageError;  ///< 当前电压量程
  uint32_t channel_data_buffer_size_ = 10 * 1024 * 1024;  ///< 通道数据大小，一般分配10M
};

struct BaseGPIOption {
  std::vector<uint8_t> gpi_state_ = {};
};

struct BaseCounterOption {
  std::vector<uint32_t> counter_state_ = {};
};
}  // namespace device
}  // namespace libdaq
