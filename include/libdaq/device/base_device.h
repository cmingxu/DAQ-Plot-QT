#pragma once

#include <condition_variable>
#include <memory>
#include <mutex>
#include <thread>
#include <atomic>
#include <future>
#include <thread>

// clang-format off
#include "readerwriterqueue.h"
#include "libdaq/backend/base_backend.h"
#include "libdaq/backend/base_backend_option.h"
#include "libdaq/device/base_device_config.h"
#include "libdaq/device/base_device_option.h"
// clang-format on

namespace libdaq {
namespace device {
using std::unique_ptr;
using std::vector;
using namespace libdaq::backend;
using namespace moodycamel;
/**
 * @brief 设备控制基类，开发者需要重点关注
 *
 * 该基类抽象了DAQ系列设备所有对外开放的方法
 *
 */
class BaseDevice {
 public:
  BaseDevice();
  virtual ~BaseDevice();

  /** @name 查询设备属性信息
   *  @brief 查询属性函数块包含了查询设备属性的相关操作。
   */
  ///@{
  /**
   * @brief 获取设备名字
   * @return 返回设备名字
   */
  const std::string &GetDeviceName() const;

  /**
   * @brief 获取底层硬件版本
   * @return
   *    - 成功，返回当前版本信息。
   *    - 失败，固定返回"0000"
   */
  virtual const std::string &GetHardwareVersion();

  /**
   * @brief 获取底层BootLoader版本
   * @return
   *    - 成功，返回当前版本信息。
   *    - 失败，固定返回"0000"
   */
  virtual const std::string &GetBootLoaderVersion();

  /**
   * @brief 获取底层软件版本
   * @return
   *    - 成功，返回当前版本信息。
   *    - 失败，固定返回"0000"
   */
  virtual const std::string &GetSoftwareVersion();
  ///@}

  /** @name 通信后端相关函数
   *
   *  @brief 不同的设备对后端的支持的情况不同，目前设备和后端对应情况如下:
   *    - DAQ122: USB2.0
   *    - DAQ216: USB3.0 TCP
   *    - DAQ217: USB3.0 TCP
   *    - DAQ329: Serial TCP
   */
  ///@{
  /**
   * @brief 使用USB作为通信后端
   * @return 是否设置成功
   */
  virtual bool UseUSBBackend();
  /**
   * @brief 使用串口作为通信后端
   * @param uart_name 串口号
   * @return 是否设置成功
   */
  virtual bool UseSerialBackend(const std::string &uart_name);
  /**
   * @brief 使用UDP服务器作为通信后端
   * @return 是否设置成功
   */
  virtual bool UseUDPBackend();
  virtual bool UseTCPClientBackend(const std::string &ip_address);
  virtual bool UseModbusRTUBackend(const std::string &uart_name);
  ///@}

  /** @name 连接设备相关函数
   *  @attention 初始化或连接设备前需要确保成功设置了设备的通信后端。
   */
  ///@{
  /**
   * @brief 初始化设备
   * @return 设备是否初始化成功
   */
  virtual bool InitializeDevice();

  /**
   * @brief 连接设备
   * @return 设备是否初始化成功
   */
  virtual bool ConnectedDevice();
  ///@}

  /** @name ADC控制相关函数
   *  @attention 使用ADC控制相关函数前需要确保成功初始化并连接了设备。
   */
  ///@{
  /**
   * @brief 配置设备的采样率和电压量程
   * @param sample_rate 采样率
   * @param voltage 电压量程
   * @return 返回值为是否配置成功。
   */
  virtual bool ConfigureADCParameters(DAQADCSampleRate sample_rate, DAQVoltage voltage);

  /**
   * @brief 使能通道状态
   * @param channel uint8_t类型的数据，8个位分别表示8个AD通道。
   * 当需要多选通道时可以对 DAQChannel 使用与操作来实现。
   * @return 是否配置成功。
   */
  virtual bool ConfigADCChannel(uint8_t channel);

  /**
   * @brief 使能通道为交流
   * @param channel uint8_t类型的数据，8个位分别表示8个AD通道。
   * 当需要多选通道时可以对 DAQChannel 使用与操作来实现。
   * @attention
   * - 需要先使用EnableChannel使能通道，再设置交流
   * - 通道只有交流和直流两种状态
   * @return 是否配置成功。
   */
  virtual bool EnableAC(uint8_t channel);

  /**
   * @brief 使能通道为直流
   * @param channel uint8_t类型的数据，8个位分别表示8个AD通道。
   * 当需要多选通道时可以对 DAQChannel 使用与操作来实现。
   * @attention
   * - 需要先使用EnableChannel使能通道，再设置直流
   * - 通道只有交流和直流两种状态
   * @return 返回值为是否配置成功。
   */
  virtual bool EnableDC(uint8_t chDnnel);

  /**
   * @brief 配置设备的电压量程
   * @param range_vector 由每一个通道的电压量程组成的vector。
   * @return 返回值为是否配置成功。
   */
  virtual bool ConfigADCVoltage(const std::vector<DAQVoltage> &range_vector);

  /**
   * @brief 配置设备的电压量程
   * @param range_vector 电压量程
   * @return 返回值为是否配置成功。
   */
  virtual bool ConfigADCVoltage(const DAQVoltage &range_vector);

  /**
   * @brief 配置设备的采样率
   * @param sample_rate 采样率
   * @return 返回值为是否配置成功。
   */
  virtual bool ConfigADCSampleRate(DAQADCSampleRate sample_rate);

  /**
   * @brief 配置设备的采样率，电压量程
   * @param range_vector 由每一个通道的电压量程组成的vector。
   * @param sample_rate 采样率
   * @return 返回值为是否配置成功。
   */
  virtual bool ConfigureADCParameters(const std::vector<DAQVoltage> &voltage_vector, DAQADCSampleRate sample_rate);
  /**
   * @brief 配置触发
   * @param type 触发类型
   * @param mode 触发模式
   * @param channel 配置触发通道
   * @param level 配置触发电平
   * @return 返回值为是否配置成功。
   */
  virtual bool ConfigADCTrigger(DAQADCTriggerType type,
                                DAQADCTriggerMode mode,
                                DAQADCTriggerChannel channel,
                                uint32_t level);

  /**
   * @brief 配置触发后采集数据的数据长度
   * @param data_length 数据长度
   * @return 返回值为是否配置成功。
   */
  virtual bool ConfigADCTriggerLength(uint32_t data_length);

  /**
   * @brief 配置触发的超时时间，即多久时间没触发后返回一次数据
   * @param timeout 触发超时时间
   * @return 返回值为是否配置成功。
   */
  virtual bool ConfigTriggerTimeout(uint32_t timeout);

  /**
   * @brief 手动触发一次采集
   * @return 返回值为是否配置成功。
   */
  virtual bool TriggerCollection();

  /**
   * @brief 设置校准参数
   * @param channel 通道
   * @param gain_calibration 增益校正参数
   * @param shifting_calibration 偏移校正参数
   * @return 返回值为是否配置成功。
   */
  virtual bool ConfigCalibration(uint8_t channel, uint32_t gain_calibration, uint32_t shifting_calibration);

  /**
   * @brief 开始采集
   * @return 返回值为是否配置成功。
   */
  virtual bool StartADCCollection();

  /**
   * @brief 停止采集
   * @return 返回值为是否配置成功。
   */
  virtual bool StopADCCollection();

  /**
   * @brief 从缓冲区内读取数据
   * @param channel 通道
   * @param read_buffer 通道写入的的数据，需要传入一个可以写的double数组头元素指针
   * @param read_size 需要读取数据大小
   * @param timeout 超时时间
   * @attention
   *    - 超时时间仅用于判断在指定的时间内缓冲区内数据是否达到读取要求，读取数据的时间不会被计算在内。
   *    - 该读取数据函数对于每一个通道是线程安全的，但同一时间只能有一个线程读取某一通道。
   * @return 是否读取成功
   */
  virtual bool TryReadADCData(uint8_t channel, double *read_buffer, uint32_t read_size, uint32_t timeout);

  /**
 * @brief 从缓冲区内读取数据
 * @param channel 通道
 * @param read_buffer 通道写入的的数据，需要传入一个可以写的double指针
 * @attention
 *    - 超时时间仅用于判断在指定的时间内缓冲区内数据是否达到读取要求，读取数据的时间不会被计算在内。
 *    - 该读取数据函数对于每一个通道是线程安全的，但同一时间只能有一个线程读取某一通道。
 * @return 是否读取成功
 */
  virtual bool TryReadADCData(uint8_t channel, double *read_buffer);

  /**
   * @brief 判断指定通道缓冲区内数据在超时的时间内是否符合要求
   * @param channel 通道
   * @param read_size 需要读取数据大小
   * @param continue_waiting 是否继续等待
   * @attention 该函数仅用于内部使用，不允许在外部调用，否则可能造成程序卡死。
   * @return 指定通道缓冲区内数据是否符合要求
   */
  bool ADCDataBufferIsValid(uint8_t channel, uint32_t read_size, const bool *continue_waiting);

  /**
   * @brief 清空设备缓冲区的数据
   * @return 是否清除成功
   */
  virtual void ClearDataBuffer();

  /**
   * @brief 获取当前设备采样率
   * @return 返回当前已经设置的采样率
   */
  virtual int GetCurrentSampleRate();

  /**
   * @brief 获取设备可以配置的合法采样率
   * @return 设备可以配置的采样率，一个列表
   */
  const std::vector<DAQADCSampleRate> &GetLegalSampleRate() const;

  /**
   * @brief 获取设备可以配置的合法电压量程
   * @return 设备可以配置的电压量程，一个列表
   */
  const std::vector<DAQVoltage> &GetLegalVoltage() const;

  /**
   * @brief 获取设备可以配置的合法通道数量
   * @return 设备可以配置的电压量程，一个列表
   */
  uint8_t GetLegalChannelSize() const;

  /**
   * @brief 获取缓冲区内数据数量
   * @param channel 指定通道
   * @return 返回指定通道的数据大小
   */
  uint32_t GetBufferDataSize(int channel);

  /**
   * @brief 查看采样率是否符合设备需求
   * @param sample_rate 需要配置的采样率
   * @return 符合返回true，否则返回false
   */
  bool SampleRateIsLegal(const DAQADCSampleRate &sample_rate);

  /**
   * @brief 查看电压量程是否符合设备需求
   * @param range 需要配置的电压
   * @return 符合返回true，否则返回false
   */
  bool RangeIsLegal(const DAQVoltage &range);

  virtual bool ADCDataIsReady(uint32_t data_num);
  ///@}

  /** @name DAC控制相关函数
   *
   *  @brief 使用DAC控制相关函数前需要确保成功初始化并连接了设备。
   */
  ///@{
  /**
   * @brief 配置DAC模式
   *
   * 该函数仅用于配置DAC直流和ADC触发源模式，
   * data表示的含义根据mode的变动而变动:
   *    - 当mode为直流时，data表示直流电平，需要传入size为1的double vector
   *    - 当mode为ADC触发源时，data表示触发电平，需要传入size为1的double vector
   *    - 当mode为波形时，data[0]表示波形频率，data[1-2000]
   * 波形数据，需要传入size为2001的double vector
   *
   * @param channel DAC通道
   * @param mode DAC模式
   * @param data DAC参数
   * @return 是否配置成功
   */
  virtual bool ConfigDAC(const DAQDACChannel &channel, const DAQDACMode &mode, const std::vector<double> &data);
  ///@}

  /** @name GPIO控制相关函数
   *
   *  @brief 使用GPIO控制相关函数前需要确保成功初始化并连接了设备。
   */
  ///@{
  /**
   * @brief 配置GPO
   * @param gpo_channel GPO通道，可以 DAQChannel 做与运算
   * @return
   */
  virtual bool ConfigGPO(int gpo_channel);
  /**
   * @brief ReadGPI
   * @return
   */
  virtual std::vector<uint8_t> ReadGPI();
  ///@}

  /** @name PWM控制相关函数
   *
   *  @brief 使用PWM控制相关函数前需要确保成功初始化并连接了设备。
   */
  ///@{
  virtual bool ConfigPWM(DAQPWMChannel channel, uint32_t frequency, uint16_t duty_cycle);
  virtual bool StartPWM(DAQPWMChannel channel);
  virtual bool StopPWM(DAQPWMChannel channel);
  ///@}

  /** @name 计数器控制相关函数
   *
   *  @brief 使用计数器控制相关函数前需要确保成功初始化并连接了设备。
   */
  ///@{
  virtual bool ConfigCounter(DAQCounterChannel channel, DAQCounterMode mode, DAQCounterTriggerType type);
  virtual bool StartCounter(DAQCounterChannel channel);
  virtual bool StopCounter(DAQCounterChannel channel);
  virtual bool ClearCounter(DAQCounterChannel channel);
  virtual std::vector<uint32_t> ReadCounter();
  ///@}



 protected:
  /** @name 自定义设备属性
   */
  ///@{
  BaseDeviceOption device_option_;
  BasePacketOption packet_option_;
  BaseADCOption adc_option_;
  BaseCounterOption counter_option_;
  BaseGPIOption gpi_option_;
  ///@}

  /** @name ADC接收和解析原始数据线程相关函数
   */
  ///@{
  std::condition_variable scroll_thread_cond_;
  std::mutex scroll_thread_mutex_;

  bool receive_thread_is_running_ = false;
  std::thread *receive_thread_ = nullptr;
  virtual bool ReceiveData(int timeout);
  virtual void ReceiveThreadFunction();
  bool OpenReceiveThread();
  bool CloseReceiveThread();

  bool parse_thread_is_running_ = false;
  std::thread *parse_thread_ = nullptr;
  virtual bool ReceivePacketHeaderIsRight(uint8_t *pointer);
  virtual bool ReceivePacketTailIsRight(uint8_t *pointer);
  virtual bool ParseData(const unique_ptr<uint8_t> &raw_data_buffer,
                         int raw_data_size);
  virtual void ParseThreadFunction();
  bool OpenParseThread();
  bool CloseParseThread();

  virtual bool OpenReceiveAndParseThread();
  virtual bool CloseReceiveAndParseThread();

  virtual bool GetSoftwareVersionFunction(uint8_t *);
  virtual bool GetHardwareVersionFunction(uint8_t *);
  virtual bool GetBootLoaderFunction(uint8_t *);
  virtual bool GetADCFunction(uint8_t *);
  virtual bool GetGPIFunction(uint8_t *);
  virtual bool GetCounterFunction(uint8_t *);
  ///@}

  /** @name 自定设备后端
   */
  ///@{
  DAQBackend backend_ = DAQBackend::Unknown;
  BaseBackendOption backend_option_;
  BaseBackend *base_backend_ = nullptr;
  ///@}

  /**
   * @name 发送命令函数块
   */
  ///@{
  uint8_t packet_index_ = 0;
  size_t CalculatePacketLength(size_t data_size);
  virtual std::shared_ptr<uint8_t> BuildDataPacket(uint16_t command, const std::vector<uint8_t> &packet_data);
  virtual bool SendCommand(uint16_t command, const std::vector<uint8_t> &data);
  virtual bool ReceivePacket(int receive_packet_length);

  virtual bool SendGetHardwareVersion();
  virtual bool SendGetBootLoaderVersion();
  virtual bool SendGetSoftwareVersion();

  virtual bool SendConnected();
  virtual bool SendConfigADCSampleRate(const DAQADCSampleRate &sample_rate);
  virtual bool SendConfigADCVoltageRange(const vector<DAQVoltage> &range_vector);
  virtual bool SendEnableChannel(uint8_t channel);
  virtual bool SendEnableAC(uint8_t channel);
  virtual bool SendEnableDC(uint8_t channel);
  virtual bool SendConfigTrigger(DAQADCTriggerType type,
                                 DAQADCTriggerMode mode,
                                 DAQADCTriggerChannel channel,
                                 uint32_t level);
  virtual bool SendConfigReceiveDataLength(uint32_t data_length);
  virtual bool SendConfigTriggerTimeout(uint32_t timeout);
  virtual bool SendTriggerCollection();
  virtual bool SendConfigCalibration(uint8_t channel, uint32_t gain_calibration, uint32_t shifting_calibration);
  virtual bool SendStartCollection();
  virtual bool SendStopCollection();

  virtual bool SendConfigDAC(const DAQDACChannel &channel, const DAQDACMode &mode, const std::vector<double> &data);

  virtual bool SendReadGPI();
  virtual bool SendConfigGPO(uint8_t do_channel);

  virtual bool SendConfigPWM(DAQPWMChannel channel, uint32_t frequency, uint16_t duty_cycle);
  virtual bool SendStartPWM(DAQPWMChannel channel);
  virtual bool SendStopPWM(DAQPWMChannel channel);

  virtual bool SendConfigCounter(DAQCounterChannel channel, DAQCounterMode mode, DAQCounterTriggerType type);
  virtual bool SendStartCounter(DAQCounterChannel channel);
  virtual bool SendStopCounter(DAQCounterChannel channel);
  virtual bool SendClearCounter(DAQCounterChannel channel);
  virtual bool SendReadCounter();
  ///@}

  /** @name 缓冲区函数块
   */
  ///@{
  ///< 原始数据缓冲区，一般分配2MB
  ReaderWriterQueue<unique_ptr<uint8_t>> *u8_raw_data_buffer_ = nullptr;

  ///< 通道数据缓冲区
  std::vector<ReaderWriterQueue<double> *> channel_data_buffer_list_;
  ///@}
};
}  // namespace device
}  // namespace libdaq
