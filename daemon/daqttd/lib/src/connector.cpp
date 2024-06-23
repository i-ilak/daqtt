#include "daqtt/connector.h"
#include <memory>
#include <mqtt/connect_options.h>
#include <mqtt/create_options.h>
#include <mqtt/exception.h>

namespace daqtt {
Connector::Connector(const ConnectionDescriptor& connection)
    : _keepAlivePeriod(connection.period * connection.max_buffered_messages)
    , _client(connection.address, connection.clientId, connection.max_buffered_messages)
{
    mqtt::connect_options connectionOptions = mqtt::connect_options_builder()
                                                  .keep_alive_interval(_keepAlivePeriod)
                                                  .clean_session(true)
                                                  .automatic_reconnect(true)
                                                  .mqtt_version(0)
                                                  .finalize();
    connectionOptions.set_user_name(connection.username);
    connectionOptions.set_password(connection.password);
    _connectionOptions = connectionOptions;

    _client.connect(connectionOptions)->wait();
}

mqtt::async_client& Connector::client()
{
    return _client;
}

mqtt::connect_options Connector::connectionOptions() const
{
    return _connectionOptions;
}

Connector::~Connector()
{
    _client.disconnect()->wait();
}
}
