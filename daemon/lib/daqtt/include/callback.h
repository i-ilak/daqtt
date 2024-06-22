#pragma once

#include <memory>
#include <mqtt/async_client.h>
#include <mqtt/connect_options.h>

namespace daqtt {
using callback_t = std::function<void(const std::string& topic, const std::string& message)>;
class ActionListener : public virtual mqtt::iaction_listener {
public:
    ActionListener(const std::string& name)
        : _name(name)
    {
    }
    ActionListener() = default;

private:
    void on_failure(const mqtt::token& tok) override
    {
        std::cout << _name << " failure";
        if (tok.get_message_id() != 0)
            std::cout << " for token: [" << tok.get_message_id() << "]" << std::endl;
        std::cout << std::endl;
    }

    void on_success(const mqtt::token& tok) override
    {
        std::cout << _name << " success";
        if (tok.get_message_id() != 0)
            std::cout << " for token: [" << tok.get_message_id() << "]" << std::endl;
        auto top = tok.get_topics();
        if (top && !top->empty())
            std::cout << "\ttoken topic: '" << (*top)[0] << "', ..." << std::endl;
        std::cout << std::endl;
    }

private:
    std::string _name;
};

class Callback : public virtual mqtt::callback,
                 public virtual mqtt::iaction_listener {
public:
    Callback(const callback_t&& callback, mqtt::async_client* client, mqtt::connect_options connectionOptions)
        : _callback(std::move(callback))
        , _subListener("Listener")
        , _client(client)
        , _connectionOptions(connectionOptions)
    {
    }
    Callback() = default;

private:
    void reconnect()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(2500));
        try {
            _client->connect(_connectionOptions, nullptr, *this);
        } catch (const mqtt::exception& exc) {
            std::cerr << "Error: " << exc.what() << std::endl;
            exit(1);
        }
    }

    void on_failure(const mqtt::token& tok) override
    {
        std::cout << "Connection attempt failed" << std::endl;
        if (++_nretry > 10)
            exit(1);
        reconnect();
    }

    void on_success(const mqtt::token& tok) override { }

    void connected(const std::string& cause) override
    {
        std::cout << "\nConnection success" << std::endl;
        _client->subscribe("#", 1, nullptr, _subListener);
    }

    void connection_lost(const std::string& cause) override
    {
        std::cout << "\nConnection lost" << std::endl;
        if (!cause.empty())
            std::cout << "\tcause: " << cause << std::endl;

        std::cout << "Reconnecting..." << std::endl;
        _nretry = 0;
        reconnect();
    }

    void message_arrived(mqtt::const_message_ptr msg) override
    {
        _callback(msg->get_topic(), msg->to_string());
    }

    void delivery_complete(mqtt::delivery_token_ptr token) override { }

private:
    int _nretry;
    ActionListener _subListener;
    callback_t _callback;
    mqtt::async_client* _client;
    mqtt::connect_options _connectionOptions;
};

}
