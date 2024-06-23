#include <iostream>

#include <daqtt/daqtt.h>
#include "influxdb.hpp"

influxdb_cpp::server_info
    si("influxdb", 8086, "daqtt_org",
       "@@INFLUXDB_INIT_TOKEN@@",
       "daqtt");

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

int main() {
  const daqtt::ConnectionDescriptor connectionBroker = {
      .address = "tcp://mosquitto:1883",
      .clientId = "daq_zmq_subscribder",
      .max_buffered_messages = 120,
      .period = std::chrono::seconds(5),
      .username = "@@MOSQUITTO_USERNAME@@",
      .password = "@@MOSQUITTO_PASSWORD@@",
  };

  daqtt::Subscriber subscriber(connectionBroker, to_cout, "#");

  while (std::tolower(std::cin.get()) != 'q')
    ;
}
