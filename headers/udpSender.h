#ifndef __UDPSENDER_H__
#define __UDPSENDER_H__

#include <arpa/inet.h>
#include <cstring>
#include <memory>
#include <vector>

namespace udp_sender {

  class udpSender {
  private:
    std::string m_address;
    unsigned long int m_port;
    int m_socket;
    sockaddr_in m_si_me;
    std::shared_ptr<std::vector<char>> buffer;

  public:
    udpSender(const std::string &address, const unsigned long int &port);

    ~udpSender();

    void sendData(std::shared_ptr<std::vector<char>> data);
  };

} // namespace udp_sender

#endif //__UDPSENDER_H__
