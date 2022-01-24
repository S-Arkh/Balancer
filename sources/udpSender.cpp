#include "udpSender.h"
#include "balancerExceptions.h"

#include <arpa/inet.h>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

udp_sender::udpSender::udpSender(const std::string &address, const unsigned long int &port)
    : m_address(address), m_port(port) {

  if ((m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
    throw configReaderException("Can't create socket");
  }

  memset((char *)&m_si_me, 0, sizeof(m_si_me));
  m_si_me.sin_family = AF_INET;
  m_si_me.sin_port = htons(port);

  if (inet_aton(m_address.c_str(), &m_si_me.sin_addr) == 0) {
    throw configReaderException("Can't convert address");
  }
};

udp_sender::udpSender::~udpSender() {
  close(m_socket);
};

void udp_sender::udpSender::sendData(std::shared_ptr<std::vector<char>> data, const long int &length) {

  int slen = sizeof(m_si_me);

  if (sendto(m_socket, data->data(), length, 0, (struct sockaddr *)&m_si_me, slen) == -1) {
    throw configReaderException("Can't send data to server");
  }
};

std::shared_ptr<std::string> udp_sender::udpSender::getServerinfo() {
  return std::make_shared<std::string>(m_address + " : " + std::to_string(m_port));
}
