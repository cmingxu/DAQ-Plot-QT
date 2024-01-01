#pragma once

#include <vector>

extern "C" {
#include "event2/event.h"
#include "event2/bufferevent.h"
}
#include "libdaq/backend/base_backend.h"
#include "libdaq/backend/base_backend_option.h"

namespace libdaq
{
    namespace backend
    {
        struct TCPSocket
        {
            evutil_socket_t fd{};
            std::string address;
            int port;
        };

        class TCPClientBackend : public BaseBackend
        {
        public:
            TCPClientBackend() = default;

            ~TCPClientBackend() override;

            bool Initialize(const BaseBackendOption &runtime_option) override;

            bool Open() override;

            bool ReadData(unsigned char *data, int read_size, int *actual_size,
                          int timeout) override;

            bool WriteData(unsigned char *data, int write_size, int *actual_size, int timeout) override;

        private:
            TCPSocketOption tcp_socket_option_;

            event_base *event_base_ = nullptr;
            TCPSocket tcp_socket_ = {};

            bool CreateEvutilSocket(TCPSocket &tcp_socket);
        };
    }
}
