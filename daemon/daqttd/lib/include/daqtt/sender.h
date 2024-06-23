#pragma once

#include <memory>
#include <string>
#include <vector>

#include "connection_descriptor.h"

namespace daqtt {
class SenderImpl;

class Sender {
public:
    Sender(const ConnectionDescriptor& connection);

public:
    void publish(const std::string& topic, const std::string&& payload);
    const std::vector<std::string> topics() const;

public:
    ~Sender();

private:
    std::unique_ptr<SenderImpl> _impl;
};
}
