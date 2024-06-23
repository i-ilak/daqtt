#include <chrono>
#include <daqtt/daqtt.h>

#include <iostream>
#include <cmath>
#include <numbers>
#include <thread>
#include <vector>

int main() {
  const daqtt::ConnectionDescriptor connectionBroker = {
      .address = "tcp://localhost:1883",
      .clientId = "sinus_sender",
      .max_buffered_messages = 120,
      .period = std::chrono::seconds(5),
      .username = "daqtt",
      .password = "daqtt",
  };

  std::vector<float> sinusWave;
  sinusWave.reserve(1000);
  constexpr double delta = 2 * std::numbers::pi_v<double> / (1000 - 1);
  for (std::size_t i = 1; i < 1000; i++)
    sinusWave[i] = std::sin(i * delta);

  uint64_t currentTime;
  std::size_t counter = 0;
  try {
    daqtt::Sender publisher(connectionBroker);
    while (true) {
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
      currentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(
                        std::chrono::system_clock::now().time_since_epoch())
                        .count();
      publisher.publish("data",
                        std::to_string(currentTime) + "/" +
                            std::to_string(sinusWave[counter++ % 1000]));
    }
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}
