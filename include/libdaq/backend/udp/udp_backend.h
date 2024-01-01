#pragma once

#include <vector>
extern "C" {
#include "event2/event.h"
#include "event2/bufferevent.h"
}
#include "libdaq/backend/base_backend.h"
#include "libdaq/backend/base_backend_option.h"

namespace libdaq {
namespace backend {
struct UDPSocket {
  evutil_socket_t fd{};
  std::string address;
  int port;
};

class UDPBackend : public BaseBackend {
 public:
  UDPBackend();
  ~UDPBackend() override;
  bool Initialize(const BaseBackendOption &runtime_option) override;
  bool Open() override;
  bool ReadData(unsigned char *data, int read_size, int *actual_size,
                int timeout) override;
  bool ReadData(uint8_t endpoint_index, unsigned char *data, int read_size,
                int *actual_size, int timeout) override;

  bool WriteData(unsigned char *data, int write_size, int *actual_size, int timeout) override;
  bool WriteData(uint8_t endpoint_index, unsigned char *data,
                 int write_size, int *actual_size, int timeout) override;

 private:
  UDPSocketOption udp_socket_option_;
  event_base *event_base_ = nullptr;
  std::vector<UDPSocket> udp_socket_vector_ = {};
  bool CreateEvutilSocket(UDPSocket &udp_socket);
};
}
}