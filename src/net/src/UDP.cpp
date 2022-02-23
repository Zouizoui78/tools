#include "tools/net/UDP.hpp"

#include <iostream>
#include <string.h> // For strerror()
#include <unistd.h> // For close()

namespace tools::net {

namespace udp_log {
    static auto logger = tools::utils::new_logger("UDP");
}

using namespace udp_log;



UDP::UDP(uint16_t port) {
    _socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (_socket == -1) {
        logger->error("Failed to create socket.");
        _ok = false;
    }

    _addr.sin_addr.s_addr = INADDR_ANY;
    _addr.sin_port = htons(port);
    _addr.sin_family = AF_INET;

    if (bind(_socket, reinterpret_cast<sockaddr *>(&_addr), sizeof(_addr)) != 0) {
        logger->error("Failed to bind socket.");
        _ok = false;
    }

    _listen_buffer = (uint8_t *)calloc(UDP_MAX_PACKET_SIZE, sizeof(uint8_t));

    _ok = true;
    logger->info("Opened socket on port {}.", port);
}
    
UDP::~UDP() {
    if (is_ok()) {
        shutdown(_socket, SHUT_RDWR);
        ::close(_socket);

        if (_listen_thread_running) {
            _listen_thread_running = false;
            _listen_thread.join();
        }

        _ok = false;
        logger->info("Closed socket on port {}.", ntohs(_addr.sin_port));
    }

    if (_listen_buffer != nullptr) {
        free(_listen_buffer);
    }
}

bool UDP::is_ok() {
    return _ok;
}

bool UDP::start_listen(std::function<void (uint8_t *data)> callback) {
    if (!callback) {
        logger->error("Cannot start listening, invalid callback.");
        return false;
    }

    if (_listen_thread_running) {
        logger->error("Cannot start listening, already running.");
        return false;
    }

    _listen_callback = callback;
    _listen_thread = std::thread([this]() {
        sockaddr_in from;
        socklen_t fromlen = sizeof(from);

        _listen_thread_running = true;
        while (_listen_thread_running) {
            int ret = recvfrom(_socket, _listen_buffer, UDP_MAX_PACKET_SIZE, 0, reinterpret_cast<sockaddr*>(&from), &fromlen);
            if (ret == -1) {
                logger->error("Failed to receive data : {}", strerror(errno));
            }
            else {
                _listen_callback(_listen_buffer);
            }
        }
    });

    return true;
}

} // namespace tools::net