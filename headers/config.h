#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <string>
#include <vector>

namespace config {
  struct Config {
  public:
    unsigned long int port;
    std::vector<std::pair<std::string, unsigned long int>> servers;

    unsigned long int limit;

    Config();
    ~Config();
  };
} // namespace config

#endif //__CONFIG_H__
