#include <iostream>

#include "config.h"
#include "configReader.h"

#include <exception>

int main() {
  config::Config new_config;

  config::configReader reader("test_config.txt");

  try {
    reader.readConfig(new_config);
  } catch (std::exception &exep) {
    std::cout << exep.what() << std::endl;
  }

  std::cout << "port: " << new_config.port << std::endl;

  std::cout << "servers: " << std::endl;
  int counter = 0;
  for (auto it = new_config.servers.cbegin(); it != new_config.servers.cend(); ++it, counter++) {
    std::cout << counter << ": " << *it << std::endl;
  }

  std::cout << "end" << std::endl;
}
