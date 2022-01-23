#include "configReader.h"
#include <climits>
#include <cstdlib>
#include <errno.h>
#include <exception>
#include <fstream>
#include <iostream>
#include <regex>

class configReaderException : public std::exception {
private:
  std::string m_error;

public:
  configReaderException(const std::string &error) : m_error(error){

                                                    };
  virtual const char *what() const noexcept {
    return m_error.c_str();
  }
};

config::configReader::configReader(const std::string &fileName)
    : m_fileName(fileName){

      };

config::configReader::~configReader(){

};

void config::configReader::parseData(config::Config &config, const std::string &data) {

  auto start_position = data.find("port:");

  if (data.npos == start_position) {
    throw configReaderException("Can't find port in settings file");
  }

  start_position += 5; //Add length of "port:"

  auto end_position = data.find('\n', start_position);

  if (data.npos == end_position) {
    throw configReaderException("Can't find end of port in settings file");
  }

  {
    std::string port = data.substr(start_position, end_position - start_position);

    config.port = std::strtoul(port.c_str(), nullptr, 0);

    if (ULONG_MAX == config.port) {
      if (ERANGE == errno) {
        throw configReaderException("Port value is out of range");
      }
    }

    if (0 == config.port) {
      if (port.npos != port.find_first_not_of('0')) {
        throw configReaderException("Port value is invalid");
      }
    }
  }

  start_position = data.find("servers:", end_position);

  if (data.npos == start_position) {
    throw configReaderException("Can't find servers in settings file");
  }

  start_position += 8; //Add length of "servers:"

  {
    std::regex ip_regex("(([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])");

    auto end_position = data.find(';', start_position);

    if (data.npos == end_position) {
      end_position = data.find('\n', start_position);
    }

    while (data.npos != end_position) {
      auto address = data.substr(start_position, end_position - start_position);

      if (!std::regex_match(address, ip_regex)) {
        throw configReaderException("Server ip is invlid");
      }

      config.servers.push_back(address);

      start_position = end_position + 1;

      end_position = data.find(';', start_position);

      if (data.npos == end_position) {
        end_position = data.find('\n', start_position);
      }
    }

    if (true == config.servers.empty()) {
      throw configReaderException("Servers is empty");
    }
  }
};

void config::configReader::readConfig(config::Config &config) {

  std::string readed_data;
  std::ifstream file(m_fileName, std::ios::in | std::ios::binary | std::ios::ate);
  if (file.is_open()) {
    auto size = file.tellg();
    if (-1 == size) {
      throw configReaderException("Can't work with file");
    }
    std::vector<char> memblock('0', size);
    file.seekg(0, std::ios::beg);
    file.read(memblock.data(), size);
    file.close();

    readed_data.append(memblock.data(), memblock.size());

  } else {
    throw configReaderException("Can't open file for read setting");
  }

  parseData(config, readed_data);
};
