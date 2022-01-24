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

  memset((char *)&m_socket_in, 0, sizeof(m_socket_in));
  m_socket_in.sin_family = AF_INET;
  m_socket_in.sin_port = htons(port);

  if (inet_aton(m_address.c_str(), &m_socket_in.sin_addr) == 0) {
    close(m_socket);
    throw configReaderException("Can't convert address");
  }

  m_socket_in_length = sizeof(m_socket_in);
};

udp_sender::udpSender::~udpSender() {
  close(m_socket);
};

void udp_sender::udpSender::sendData(std::shared_ptr<std::vector<char>> data, const long int &length) {

  if (sendto(m_socket, data->data(), length, 0, (struct sockaddr *)&m_socket_in, m_socket_in_length) == -1) {
    throw configReaderException("Can't send data to server");
  }
};

std::shared_ptr<std::string> udp_sender::udpSender::getServerinfo() {
  return std::make_shared<std::string>(m_address + " : " + std::to_string(m_port));
}
