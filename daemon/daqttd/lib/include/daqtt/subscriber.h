#pragma once

#include "callback.h"
#include "connection_descriptor.h"
#include "connector.h"
#include <mqtt/callback.h>

namespace daqtt {

class Subscriber {
public:
    Subscriber(const ConnectionDescriptor& connectionDescriptor, const callback_t&& callback, const std::string& topicFilter);
    ~Subscriber();

public:
    void setWatchedTopics(const std::vector<std::string>& topics);

private:
    // Callback _callback;
    Connector _connector;
};
}
