#ifndef __UDPSENDER_H__
#define __UDPSENDER_H__

#include <arpa/inet.h>
#include <cstring>
#include <memory>
#include <vector>

namespace udp_sender {

  class udpSender {
  private:
    int m_socket;
    sockaddr_in m_socket_in;
    int m_socket_in_length;
    std::string m_address;
    unsigned long int m_port;

  public:
    udpSender(const std::string &address, const unsigned long int &port);

    ~udpSender();

    void sendData(std::shared_ptr<std::vector<char>> data, const long int &length);

    std::shared_ptr<std::string> getServerinfo();
  };

} // namespace udp_sender

#endif //__UDPSENDER_H__
