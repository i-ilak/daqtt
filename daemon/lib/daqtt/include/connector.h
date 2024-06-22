#pragma once

#include "connection_descriptor.h"
#include <mqtt/async_client.h>
#include <mqtt/connect_options.h>

namespace daqtt {
class Connector {
public:
    Connector(const ConnectionDescriptor& connection);

public:
    mqtt::async_client& client();
    mqtt::connect_options connectionOptions() const;

public:
    ~Connector();

private:
    std::chrono::seconds _keepAlivePeriod;
    mqtt::connect_options _connectionOptions;
    mqtt::async_client _client;
};
}
