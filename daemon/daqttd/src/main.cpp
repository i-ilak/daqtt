#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <thread>

#include "command-line-parser.hpp"
#include "daemon.hpp"
#include "log.hpp"

#include "daqtt/connection_descriptor.h"
#include "influxdb.hpp"
#include <daqtt/daqtt.h>

influxdb_cpp::server_info si;

void to_cout(const std::string &topic, const std::string &message) {
  auto delimiter_pos = message.find('/');
  std::string timestamp_str = message.substr(0, delimiter_pos);
  std::string data_str = message.substr(delimiter_pos + 1);
  long long timestamp = std::stoll(timestamp_str);
  double data = std::stod(data_str);
  std::string resp;
  auto ret = influxdb_cpp::builder()
                 .meas(topic)
                 .field("data", data)
                 .timestamp(timestamp)
                 .post_http(si, &resp);
}

// This function will be called when the daemon receive a SIGHUP signal.
void reload() {
  // LOG_INFO("Reload function called.");
}

daqtt::ConnectionDescriptor
configToConnectionDescriptor(const rapidjson::Value &mosquitto) {
  daqtt::ConnectionDescriptor connectionBroker = {
      .clientId = "daq_zmq_subscribder",
      .max_buffered_messages = 120,
      .period = std::chrono::seconds(5),
  };
  connectionBroker.address =
      "tcp://" + std::string(mosquitto["broker_ip"].GetString()) + ":" +
      std::to_string(mosquitto["broker_port"].GetInt());
  connectionBroker.username = mosquitto["username"].GetString();
  connectionBroker.password = mosquitto["password"].GetString();
  return connectionBroker;
}

influxdb_cpp::server_info
configToInfludbConnection(const rapidjson::Value &influxdb) {
  return influxdb_cpp::server_info("influxdb", 8086, "daqtt_org",
                                   influxdb["token"].GetString(), "daqtt");
}

rapidjson::Document getConfig(const std::string &filePath) {
  LOG_INFO("Config File name:\t", filePath);
  std::string contents;
  std::ifstream infile;
  infile.open(filePath, std::ios::binary);
  if (!infile.is_open()) {
    LOG_ERROR("Could not open file for reading!");
    exit(EXIT_FAILURE);
  }
  contents.assign(std::istreambuf_iterator<char>(infile),
                  std::istreambuf_iterator<char>());

  rapidjson::Document d;
  d.Parse(contents.c_str());
  if (d.HasParseError()) {
    LOG_ERROR(
        "JSON parse error:", rapidjson::GetParseError_En(d.GetParseError()),
        "(", d.GetErrorOffset(), ")");
    exit(EXIT_FAILURE);
  }
  return d;
}

void validateConfig(const rapidjson::Document &d) {
  if (d.HasMember("mosquitto") == false || d["mosquitto"].IsObject() == false) {
    LOG_ERROR("No 'mosquitto' object found in the JSON");
    exit(EXIT_FAILURE);
  } else if (d.HasMember("influxdb") == false ||
             d["influxdb"].IsObject() == false) {
    LOG_ERROR("No 'ifluxdb' object found in the JSON");
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char **argv) {
  CommandLineParser commandLine(argc, argv);
  if (commandLine.cmdOptionExist("--config") == false) {
    LOG_ERROR("No config file provided!");
    return EXIT_FAILURE;
  }
  rapidjson::Document config =
      getConfig(commandLine.getCmdOptionValue("--config"));

  validateConfig(config);
  daqtt::ConnectionDescriptor connectionBroker =
      configToConnectionDescriptor(config["mosquitto"]);
  si = configToInfludbConnection(config["influxdb"]);
  // The Daemon class is a singleton to avoid be instantiate more than once
  Daemon &daemon = Daemon::instance();

  // Set the reload function to be called in case of receiving a SIGHUP signal
  daemon.setReloadFunction(reload);

  daqtt::Subscriber subscriber(connectionBroker, to_cout, "#");

  while (daemon.IsRunning())
    ;

  // LOG_INFO("The daemon process ended gracefully.");
}
