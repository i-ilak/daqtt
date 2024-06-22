#include "subscriber.h"
#include "connection_descriptor.h"
#include "connector.h"
#include <memory>
#include <mqtt/async_client.h>
#include <mqtt/connect_options.h>
#include <mqtt/message.h>

namespace daqtt {

Subscriber::Subscriber(const ConnectionDescriptor& connectionDescriptor, const callback_t&& callback, const std::string& topicFilter)
    : _connector(connectionDescriptor)
{
    _connector.client().set_message_callback([&callback](mqtt::message::const_ptr_t msg) {
        callback(msg->get_topic(), msg->to_string());
    });
    _connector.client().subscribe(topicFilter, 1);
}
Subscriber::~Subscriber() = default;

}
