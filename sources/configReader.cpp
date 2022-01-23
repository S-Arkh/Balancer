#include "configReader.h"
#include "balancerExceptions.h"
#include <climits>
#include <cstdlib>
#include <errno.h>
#include <fstream>
#include <iostream>
#include <regex>

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

    if (std::to_string(config.port) != port) {
      throw configReaderException("Read invalid port");
    }
  }

  start_position = data.find("servers:");

  if (data.npos == start_position) {
    throw configReaderException("Can't find servers in settings file");
  }

  start_position += 8; //Add length of "servers:"

  {
    std::regex ip_regex("(([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])");

    end_position = data.find(':', start_position);

    if (data.npos == end_position) {
      end_position = data.find('\n', start_position);
    }

    while ((data.npos != end_position) && (start_position != end_position)) {
      auto address = data.substr(start_position, end_position - start_position);

      if (!std::regex_match(address, ip_regex)) {
        throw configReaderException("Server ip is invlid");
      }

      start_position = end_position + 1;

      end_position = data.find(';', start_position);
      if (data.npos == end_position) {
        end_position = data.find('\n', start_position);
      }

      std::string server_port = data.substr(start_position, end_position - start_position);

      unsigned long int port = std::strtoul(server_port.c_str(), nullptr, 0);

      if (ULONG_MAX == port) {
        if (ERANGE == errno) {
          throw configReaderException("Server port value is out of range");
        }
      }

      if (0 == port) {
        if (server_port.npos != server_port.find_first_not_of('0')) {
          throw configReaderException("Server port value is invalid");
        }
      }

      if (std::to_string(port) != server_port) {
        throw configReaderException("Read invalid server port");
      }

      config.servers.emplace_back(address, port);

      start_position = end_position + 1;

      if (data.find(':', start_position) > data.find('\n', start_position)) {
        break;
      }
      end_position = data.find(':', start_position);

      if (data.npos == end_position) {
        end_position = data.find('\n', start_position);
      }
    }

    if (true == config.servers.empty()) {
      throw configReaderException("Servers is empty");
    }
  }

  start_position = data.find("limit:");

  if (data.npos == start_position) {
    throw configReaderException("Can't find limit in settings file");
  }

  start_position += 6; //Add length of "limit:"

  end_position = data.find('\n', start_position);

  if (data.npos == end_position) {
    throw configReaderException("Can't find end of limit in settings file");
  }

  {
    std::string limit = data.substr(start_position, end_position - start_position);

    config.limit = std::strtoul(limit.c_str(), nullptr, 0);

    if (ULONG_MAX == config.limit) {
      if (ERANGE == errno) {
        throw configReaderException("Limit value is out of range");
      }
    }

    if (0 == config.limit) {
      if (limit.npos != limit.find_first_not_of('0')) {
        throw configReaderException("Limit value is invalid");
      }
    }

    if (std::to_string(config.limit) != limit) {
      throw configReaderException("Read invalid limit");
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
    std::vector<char> memblock(size, '0');
    file.seekg(0, std::ios::beg);
    file.read(memblock.data(), size);
    file.close();

    readed_data.append(memblock.data(), memblock.size());

  } else {
    throw configReaderException("Can't open file for read setting");
  }

  parseData(config, readed_data);
};
