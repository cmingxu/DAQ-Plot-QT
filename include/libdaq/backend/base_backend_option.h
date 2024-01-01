#pragma once

#include <iostream>
#include <vector>

namespace libdaq {
namespace backend {
struct USBOption {
  USBOption() = default;
  USBOption(const USBOption &option) {
    this->vendor_id = option.vendor_id;
    this->product_id = option.product_id;
    this->endpoint_in = option.endpoint_in;
    this->endpoint_out = option.endpoint_out;
  }

  USBOption &operator=(const USBOption &option) {
    this->vendor_id = option.vendor_id;
    this->product_id = option.product_id;
    this->endpoint_in = option.endpoint_in;
    this->endpoint_out = option.endpoint_out;
    return *this;
  }

  uint16_t vendor_id = 0x00;
  uint16_t product_id = 0x00;
  std::vector<uint8_t> endpoint_in = {0x00};
  std::vector<uint8_t> endpoint_out = {0x00};
};

struct SerialOption {
  SerialOption() = default;
  SerialOption(const SerialOption &option) {
    this->port_name_ = option.port_name_;
    this->baud_rate_ = option.baud_rate_;
  }
  SerialOption &operator=(const SerialOption &option) {
    this->port_name_ = option.port_name_;
    this->baud_rate_ = option.baud_rate_;
    return *this;
  }

  std::string port_name_;
  uint32_t baud_rate_ = 0;
};

struct UDPSocketOption {
  UDPSocketOption() = default;
  UDPSocketOption(const UDPSocketOption &option) {
    this->address_ = option.address_;
    this->port_ = option.port_;
  }

  UDPSocketOption &operator=(const UDPSocketOption &option) {
    this->address_ = option.address_;
    this->port_ = option.port_;
    return *this;
  }

  std::vector<std::string> address_ = {};
  std::vector<int> port_ = {};
};

struct TCPSocketOption {
  TCPSocketOption() = default;
  TCPSocketOption(const TCPSocketOption &option) {
    this->address_ = option.address_;
    this->port_ = option.port_;
  }

  TCPSocketOption &operator=(const TCPSocketOption &option) {
    this->address_ = option.address_;
    this->port_ = option.port_;
    return *this;
  }

  std::string address_ = {};
  int port_ = {};
};

struct ModbusOption {
  ModbusOption() = default;
  ModbusOption(const ModbusOption &option) {
    this->port_name_ = option.port_name_;
    this->baud_rate_ = option.baud_rate_;
  }

  ModbusOption &operator=(const ModbusOption &option) {
    this->port_name_ = option.port_name_;
    this->baud_rate_ = option.baud_rate_;
    return *this;
  }

  std::string port_name_;
  int baud_rate_ = 0;
};

struct BaseBackendOption {
 public:
  BaseBackendOption() = default;

  void UseUSB(uint16_t vendor_id, uint16_t product_id,
              const std::vector<uint8_t> &endpoint_in,
              const std::vector<uint8_t> &endpoint_out) {
    usb_option_.vendor_id = vendor_id;
    usb_option_.product_id = product_id;
    usb_option_.endpoint_in = endpoint_in;
    usb_option_.endpoint_out = endpoint_out;
  };
  USBOption GetUSBOption() const { return usb_option_; }

  void UseSerial(const std::string &port_name, uint32_t baud_rate_) {
    serial_option_.port_name_ = port_name;
    serial_option_.baud_rate_ = baud_rate_;
  }
  SerialOption GetSerialOption() const { return serial_option_; }

  void UseUDP(const std::vector<std::string> &address, const std::vector<int> &port) {
    udp_socket_option.address_ = address;
    udp_socket_option.port_ = port;
  }
  UDPSocketOption GetUDPSocketOption() const { return udp_socket_option; }

  void UseTCP(const std::string &address, const int &port) {
    tcp_socket_option.address_ = address;
    tcp_socket_option.port_ = port;
  }
  TCPSocketOption GetTCPSocketOption() const { return tcp_socket_option; }

  void UseModbus(const std::string &port_name, uint32_t baud_rate) {
    modbus_option.port_name_ = port_name;
    modbus_option.baud_rate_ = baud_rate;
  }
  ModbusOption GetModbusOption() const { return modbus_option; }

 private:
  USBOption usb_option_;
  SerialOption serial_option_;
  UDPSocketOption udp_socket_option;
  TCPSocketOption tcp_socket_option;
  ModbusOption modbus_option;
};
}  // namespace backend
}  // namespace libdaq
