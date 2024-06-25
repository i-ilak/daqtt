#include <chrono>
#include <iostream>
#include <libconfig.h++>
#include <thread>

#include "command-line-parser.hpp"
#include "daemon.hpp"
#include "log.hpp"

#include "influxdb.hpp"
#include <daqtt/daqtt.h>

influxdb_cpp::server_info si("influxdb", 8086, "daqtt_org",
                             "@@INFLUXDB_INIT_TOKEN@@", "daqtt");

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
  // std::cout << "Topic:\t" << topic << "\t Message:\t" << message
  //           << "\tResponse:\t" << ret << ";\t" << resp << "\n";
}

// This function will be called when the daemon receive a SIGHUP signal.
void reload() { LOG_INFO("Reload function called."); }

int main(int argc, char **argv) {
  // Command line and config file example
  CommandLineParser commandLine(argc, argv);
  if (commandLine.cmdOptionExist("--config")) {
    const std::string configFileName =
        commandLine.getCmdOptionValue("--config");
    LOG_INFO("Config file name = ", configFileName);
    libconfig::Config config;
    config.readFile(configFileName.c_str());
    std::string test{};
    config.lookupValue("test", test);
    LOG_INFO("Config option test = ", test);
  }

  // The Daemon class is a singleton to avoid be instantiate more than once
  Daemon &daemon = Daemon::instance();

  // Set the reload function to be called in case of receiving a SIGHUP signal
  daemon.setReloadFunction(reload);

  const daqtt::ConnectionDescriptor connectionBroker = {
      .address = "tcp://mosquitto:1883",
      .clientId = "daq_zmq_subscribder",
      .max_buffered_messages = 120,
      .period = std::chrono::seconds(5),
      .username = "@@MOSQUITTO_USERNAME@@",
      .password = "@@MOSQUITTO_PASSWORD@@",
  };

  daqtt::Subscriber subscriber(connectionBroker, to_cout, "#");

  while (daemon.IsRunning())
    ;

  LOG_INFO("The daemon process ended gracefully.");
}
