#include <iostream>

#include "config.h"
#include "configReader.h"
#include "udpReceiver.h"

#include <chrono>
#include <exception>

int main() {
  config::Config new_config;

  config::configReader reader("test_config.txt");

  try {
    reader.readConfig(new_config);
  } catch (std::exception &exep) {
    std::cout << exep.what() << std::endl;
    return 0;
  }

  std::cout << "port: " << new_config.port << std::endl;

  std::cout << "limit: " << new_config.limit << std::endl;

  std::cout << "servers: " << std::endl;
  int counter = 0;
  for (auto it = new_config.servers.cbegin(); it != new_config.servers.cend(); ++it, counter++) {
    std::cout << counter << ": " << it->first << ':' << it->second << std::endl;
  }

  udp_receiver::udpReceiver rec(8888);

  long int recived_length;
  unsigned long int count = 0;

  std::chrono::duration<int, std::ratio<10>> one_minute(1);

  std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
  std::chrono::system_clock::time_point next = now + one_minute;
  for (;;) {
    now = std::chrono::system_clock::now();
    if (next <= now) {
      count = 0;
      next = now + one_minute;
    }
    if (count < new_config.limit) {
      auto getted_data = rec.receiveData(recived_length);

      std::cout << "getted_data: ";
      auto it = getted_data->cbegin();
      for (long int i = 0; i < recived_length; ++i) {
        std::cout << *it;
        it++;
      }
      std::cout << std::endl;
      count++;
    }
  }
  std::cout << "end" << std::endl;
}
