#pragma once

#include <string>
#include <chrono>

namespace daqtt {

struct ConnectionDescriptor {
    std::string address;
    std::string clientId;
    uint8_t max_buffered_messages;
    std::chrono::seconds period;
    std::string username;
    std::string password;
};
}
