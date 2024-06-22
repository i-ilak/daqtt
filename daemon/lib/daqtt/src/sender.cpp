#include "sender.h"

#include "connection_descriptor.h"
#include "connector.h"
#include "utils.h"
#include <algorithm>
#include <chrono>
#include <unordered_map>

namespace daqtt {

class SenderImpl {
public:
    SenderImpl(const ConnectionDescriptor& connection)
        : _connector(connection)
    {
    }

    void publish(const std::string& topic, const std::string&& payload)
    {
        if (daqtt::util::contains(_topicKeys, topic) == false) {
            _topicKeys.push_back(topic);
            _topics[topic] = std::make_unique<mqtt::topic>(_connector.client(), topic, 1);
        }
        _topics[topic]->publish(std::move(payload));
    }

    std::vector<std::string> topics() const
    {
        return _topicKeys;
    }

private:
    Connector _connector;
    std::vector<std::string> _topicKeys;
    std::unordered_map<std::string, std::unique_ptr<mqtt::topic>> _topics;
};

Sender::Sender(const ConnectionDescriptor& connectionDescriptor)
    : _impl(std::make_unique<SenderImpl>(connectionDescriptor))
{
}

void Sender::publish(const std::string& topic, const std::string&& payload)
{
    _impl->publish(topic, std::move(payload));
}

const std::vector<std::string> Sender::topics() const
{
    return _impl->topics();
}

Sender::~Sender() = default;

}
