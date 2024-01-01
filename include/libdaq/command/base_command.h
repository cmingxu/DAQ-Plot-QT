#pragma once

#include <iostream>
#include <vector>

namespace libdaq {
namespace device {
namespace command {

/**
 * @brief 基本信息（用户只读）
 */
enum class BasicInformation {
  DeviceName = 0x0000,               /// 获取设备名称
  DeviceHardwareVersion = 0x0001,    /// 获取硬件版本
  DeviceBootLoaderVersion = 0x0002,  /// 获取BootLoader版本
  DeviceSoftwareVersion = 0x0003,    /// 获取软件版本
};

/**
 * @brief ADC配置
 */
enum class ADCConfiguration {
  ConfigSampleRate = 0x0200,  ///<设置采样率
  ConfigRange = 0x0201,       ///<设置量程
  ConfigChannel = 0x020A,     ///<通道使能
  ConfigTrigger = 0x0210,     ///<设置触发方式
  StartCollection = 0x0221,   ///<开始采集
  StopCollection = 0x0222,    ///<停止采集
  GetADCData = 0x0223,  ///< 上传ADC数据流（仅用于返回数据包）

  ConfigACDC = 0x020B,               ///<设置采样率
  ConfigReceiveDataLength = 0x0211,  ///<设置采样率
  ConfigTriggerTimeout = 0x0212,     ///<设置采样率
  ConfigCalibration = 0x02E0,        ///<设置采样率
};

/**
 * @brief 系统操作（用户可读可写）
 */
enum class SystemOperation {
  ResetByFPGA = 0x0301,
  RestoreFactoryConfiguration = 0x0311,
  UpdateFirmware = 0x0321,
  EraseFirmware = 0x0322,
  ReceiveFirmware = 0x0333,
  ConnectDevice = 0x03FF,
  DisconnectDevice = 0x03FE,
  ResetByUSB = 0x0302,
};

/**
 * @brief DAC配置
 */
enum class DACConfiguration {
  ConfigDAC = 0X0400,  ///< 设置DAC通道，模式，数据
  StartDAC = 0x0401,   ///< 启动DAC
  StopDAC = 0x0402,    ///< 停止DAC
};

/**
 * @brief DIO配置
 */
enum class GPIOConfiguration {
  SetGPO = 0x0500,  ///< 设置GPO，单次操作
  GetGPI = 0x0501,  ///< 获取GPI状态
};

/**
 * @brief PWM配置
 */
enum class PWMConfiguration {
  ConfigPWM = 0x0600,  ///< 配置PWM
  StartPWM = 0x0601,   ///< 启动PWM
  StopPWM = 0x0602,    ///< 停止PWM
};

/**
 * @brief 计数器配置
 */
enum class CounterConfiguration {
  ConfigCounter = 0x0700,  ///< 配置计数器
  GetCounter = 0x0701,     ///< 获取计数值
  ClearCounter = 0x0702,   ///< 清零计数值
  StartCounter = 0x0703,   ///< 启动计数器
  StopCounter = 0x0704,    ///< 停止计数器
};

/**
 * @brief 生产测试（内部）
 * @attention 该部分命令仅供内部测试
 */
enum class ProductionTestingConfiguration {
  SetSN = 0x0331,  ///< 设置SN
  GetSN = 0x0332,  ///< 获取SN
};
}  // namespace command
}  // namespace device
}  // namespace libdaq
