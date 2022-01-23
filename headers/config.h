#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <string>
#include <vector>

namespace config {
  struct Config {
  public:
    unsigned long int port;
    std::vector<std::string> servers;

    Config();
    ~Config();
  };
} // namespace config

#endif //__CONFIG_H__
