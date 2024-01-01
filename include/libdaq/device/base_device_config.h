#pragma once
namespace libdaq {
namespace device {
/**
 * @brief DAQ设备后端
 */
enum class DAQBackend {
  Unknown,     ///< 未知后端
  USB,         ///< USB后端
  SerialPort,  ///< 串口后端
  UDPServer,
  TCPClient,
  ModbusRTU,
};

/**
 * @brief 电压量程枚举配置
 *
 * ADC和DAC统一使用该电压量程枚举
 */
enum class DAQVoltage {
  VoltageError = 13,  ///< 未知电压量程
  Voltage100MV = 0,   ///< 100mV电压量程
  Voltage200MV = 1,   ///< 200mV电压量程
  Voltage500MV = 2,   ///< 500mV电压量程
  Voltage1V = 3,      ///< 1V电压量程
  Voltage2V = 4,      ///< 2V电压量程
  Voltage2V5 = 5,     ///< 2.5V电压量程
  Voltage5V = 6,      ///< 5V电压量程
  Voltage10V = 7,     ///< 10V电压量程
  Voltage15V = 8,     ///< 15V电压量程
  Voltage20V = 9,     ///< 20V电压量程
  Voltage50V = 10,    ///< 50V电压量程
  Voltage60V = 11,    ///< 60V电压量程
  Voltage100V = 12,   ///< 100V电压量程
};

/**
 * @brief ADC采样率配置
 */
enum class DAQADCSampleRate {
  SampleRateError = 0,         ///< 未知采样率
  SampleRate10 =   10,         ///< 10采样率
  SampleRate100 = 100,         ///< 100采样率
  SampleRate500 = 500,         ///< 500采样率
  SampleRate1K = 1000,         ///< 1K采样率
  SampleRate5K = 5000,         ///< 5K采样率
  SampleRate10K = 10000,       ///< 10K采样率
  SampleRate50K = 50000,       ///< 50K采样率
  SampleRate100K = 100000,     ///< 100K采样率
  SampleRate200K = 200000,     ///< 200K采样率
  SampleRate500K = 500000,     ///< 500K采样率
  SampleRate1M = 1000000,      ///< 1M采样率
  SampleRate2M = 2000000,      ///< 2M采样率
  SampleRate5M = 5000000,      ///< 5M采样率
  SampleRate10M = 10000000,    ///< 10M采样率
  SampleRate20M = 20000000,    ///< 20M采样率
  SampleRate50M = 50000000,    ///< 50M采样率
  SampleRate65M = 65000000,    ///< 65M采样率
  SampleRate100M = 100000000,  ///< 100M采样率
  SampleRate125M = 125000000,  ///< 125M采样率
  SampleRate250M = 250000000,  ///< 250M采样率
};

/**
 * @brief ADC通道配置
 * @attention 多选通道，可以使用与操作实现
 */
enum class DAQADCChannel {
  None = 0b00000000,       /// 全不选
  AIN1 = 0b00000001,       /// ADC通道1
  AIN2 = 0b00000001 << 1,  /// ADC通道2
  AIN3 = 0b00000001 << 2,  /// ADC通道3
  AIN4 = 0b00000001 << 3,  /// ADC通道4
  AIN5 = 0b00000001 << 4,  /// ADC通道5
  AIN6 = 0b00000001 << 5,  /// ADC通道6
  AIN7 = 0b00000001 << 6,  /// ADC通道7
  AIN8 = 0b00000001 << 7,  /// ADC通道8
};

/**
 * @brief ADC触发类型配置
 */
enum class DAQADCTriggerType {
  Normal = 0x00,       ///< 正常模式或滚动模式（不进行触发）
  RisingEdge = 0x01,   ///< 上升沿
  FallingEdge = 0x02,  ///< 下降沿
  DoubleEdge = 0x03,   ///<  双边缘
  Error = 0x04,
};

/**
 * @brief ADC触发模式配置
 */
enum class DAQADCTriggerMode {
  Automatic = 0x00,
  Common = 0x01,
  Single = 0x02,
  Error = 0x04,
};

/**
 * @brief ADC触发通道配置
 * @attention 多选通道，可以使用与操作实现
 */
enum class DAQADCTriggerChannel {
  SoftwareTrigger = 0x00,  /// 软件触发
  AIN1 = 0b00000001,       /// ADC通道1
  AIN2 = 0b00000001 << 1,  /// ADC通道2
  AIN3 = 0b00000001 << 2,  /// ADC通道3
  AIN4 = 0b00000001 << 3,  /// ADC通道4
  AIN5 = 0b00000001 << 4,  /// ADC通道5
  AIN6 = 0b00000001 << 5,  /// ADC通道6
  AIN7 = 0b00000001 << 6,  /// ADC通道7
  AIN8 = 0b00000001 << 7,  /// ADC通道8
};

/**
 * @brief DAC通道配置
 */
enum class DAQDACChannel {
  DAC1 = 0x01,  /// DAC通道1
  DAC2 = 0x02,  /// DAC通道2
};

/**
 * @brief DAC模式配置
 */
enum class DAQDACMode {
  DirectCurrent = 0x01,  /// 直流
  Wave = 0x02,           /// 波形
  TriggerSource = 0x03,  /// 触发源
};

/**
 * @brief DI通道配置
 * @attention 多选通道，可以使用与操作实现
 */
enum class DAQDIChannel {
  DI1 = 0b00000001,       /// DI通道1
  DI2 = 0b00000001 << 1,  /// DI通道2
  DI3 = 0b00000001 << 2,  /// DI通道3
  DI4 = 0b00000001 << 3,  /// DI通道4
  DI5 = 0b00000001 << 4,  /// DI通道5
  DI6 = 0b00000001 << 5,  /// DI通道6
};

/**
 * @brief PWM通道配置
 */
enum class DAQPWMChannel {
  PWM1 = 1,  /// PWM通道1
  PWM2 = 2,  /// PWM通道2
};

/**
 * @brief 计数器通道配置
 */
enum class DAQCounterChannel {
  Counter1 = 1,  /// Counter通道1
  Counter2 = 2,  /// Counter通道2
};

/**
 * @brief 计数器模式配置
 */
enum class DAQCounterMode {
  Counter = 0x01,               /// 计数模式
  FrequencyMeasurement = 0x02,  /// 测频
  PWM = 0x03,                   /// PWM
};

/**
 * @brief 计数器触发类型配置
 */
enum class DAQCounterTriggerType {
  RisingEdge = 0x01,   ///< 上升沿
  FallingEdge = 0x02,  ///< 下降沿
  DoubleEdge = 0x03,   ///<  双边缘
};

}  // namespace device
}  // namespace libdaq
