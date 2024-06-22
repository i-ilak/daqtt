#include <iostream>

#include <daqtt/daqtt.h>
#include "influxdb.hpp"

influxdb_cpp::server_info
    si("127.0.0.1", 8086, "daqtt_org",
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
      // .address = "tcp://192.168.1.62:1883",
      .address = "tcp://localhost:1883",
      .clientId = "daq_zmq_subscribder",
      .max_buffered_messages = 120,
      .period = std::chrono::seconds(5),
      .username = "fhnw",
      .password = "fhnw",
  };

  daqtt::Subscriber subscriber(connectionBroker, to_cout, "#");

  while (std::tolower(std::cin.get()) != 'q')
    ;
}
