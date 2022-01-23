#ifndef __CONFIGREADER_H__
#define __CONFIGREADER_H__

#include "config.h"
#include <string>

namespace config {

  class configReader {
  private:
    std::string m_fileName;

    void parseData(config::Config &config, const std::string &data);

  public:
    configReader(const std::string &fileName);

    ~configReader();

    void readConfig(config::Config &config);
  };
} // namespace config

#endif //__CONFIGREADER_H__
