#include "config.h"
#include "configReader.h"
#include "udpReceiver.h"
#include "udpSender.h"

#include <chrono>
#include <ctime>
#include <exception>
#include <iostream>
#include <queue>
#include <vector>

void balancer(const config::Config &new_config) {
  udp_receiver::udpReceiver receiver(new_config.port);

  long int received_length;

  std::vector<std::unique_ptr<udp_sender::udpSender>> servers;
  try {
    for (auto it = new_config.servers.cbegin(); it != new_config.servers.cend(); ++it) {
      servers.emplace_back(std::make_unique<udp_sender::udpSender>(it->first, it->second));
    }
  } catch (std::exception &exce) {
    std::cerr << exce.what() << std::endl;
    return;
  }

  auto server_it = servers.begin();

  std::queue<std::chrono::high_resolution_clock::time_point> sended_times;
  for (;;) {

    std::shared_ptr<std::vector<char, std::allocator<char>>> getted_data;
    try {
      getted_data = receiver.receiveData(received_length);
    } catch (std::exception &exce) {
      std::cerr << exce.what() << std::endl;
      return;
    }

    if (false == sended_times.empty()) {
      std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - sended_times.front();

      if (std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() >= 1000) {
        sended_times.pop();
      }
    }

    if (sended_times.size() < new_config.limit) {
      try {
        server_it->get()->sendData(getted_data, received_length);
      } catch (std::exception &exce) {
        std::cerr << exce.what() << std::endl;
        return;
      }

      sended_times.push(std::chrono::high_resolution_clock::now());

      std::cout << "sended to server: "
                << *server_it->get()->getServerinfo()
                << std::endl;

      server_it++;
      if (server_it == servers.end()) {
        server_it = servers.begin();
      }
    }
  }
};

int main(int argc, char *argv[]) {
  std::string file_name;

  if (argc > 1) {
    file_name = argv[1];
  } else {
    std::cerr << "Don't write file name" << std::endl;
    return 0;
  }

  config::Config new_config;

  try {
    config::configReader reader(file_name);
    reader.readConfig(new_config);
  } catch (std::exception &exce) {
    std::cerr << exce.what() << std::endl;
    return 0;
  }

  std::cout << "port: " << new_config.port << std::endl;

  std::cout << "limit: " << new_config.limit << std::endl;

  std::cout << "servers: " << std::endl;
  int counter = 0;
  for (auto it = new_config.servers.cbegin(); it != new_config.servers.cend(); ++it, counter++) {
    std::cout << counter << ": " << it->first << ':' << it->second << std::endl;
  }

  try {
    balancer(new_config);
  } catch (std::exception &exce) {
    std::cerr << exce.what() << std::endl;
    return 0;
  }

  std::cout << "Balancer ended" << std::endl;
}
