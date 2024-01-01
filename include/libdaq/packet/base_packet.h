#pragma once

#include <cstring>
#include <iostream>
#include <memory>
#include <vector>

namespace libdaq {
namespace device {
namespace package {
using std::unique_ptr;
/**
 * @brief 通用原始数据数据包
 *
 * 通用原始数据包是为了解决内存拷贝问题
 *
 * @todo 修改为智能指针的形式
 * @attention 仅供内部人员二次开发使用
 */
struct RawDataPacket {
 public:
  RawDataPacket() = default;
  RawDataPacket(const RawDataPacket& other) {
    auto length = other.data_length_;
    if (length == 0) {
      data_length_ = 0;
      data_ptr_ = nullptr;
    } else {
      DeletePacket();
      BuildPacket(length);
      memcpy(data_ptr_, other.data_ptr_, sizeof(uint8_t) * data_length_);
    }
  }
  ~RawDataPacket() = default;

  bool BuildPacket(uint32_t size);
  void DeletePacket();

  uint8_t* GetData();
  const uint8_t* GetConstData();

  uint32_t GetSize();
  const uint32_t GetConstSize();

  RawDataPacket& operator=(RawDataPacket other) {
    data_ptr_ = other.data_ptr_;
    data_length_ = other.data_length_;
    return *this;
  }

 private:
  uint8_t* data_ptr_ = nullptr;
  uint32_t data_length_ = 0;
};
}  // namespace package
}  // namespace device
}  // namespace libdaq
