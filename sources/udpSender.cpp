#include "udpSender.h"
#include "balancerExceptions.h"

#include <arpa/inet.h>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

udp_sender::udpSender::udpSender(const std::string &address, const unsigned long int &port)
    : m_address(address), m_port(port), buffer(std::make_shared<std::vector<char>>(512, '0')) {

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

void udp_sender::udpSender::sendData(std::shared_ptr<std::vector<char>> data) {

  sockaddr_in si_other;
  int slen = sizeof(si_other);

  if (sendto(m_socket, data->data(), data->size(), 0, (struct sockaddr *)&si_other, slen) == -1) {
    throw configReaderException("Can't send data to server");
  }
};
