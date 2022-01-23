#include "udpReceiver.h"
#include "balancerExceptions.h"

#include <arpa/inet.h>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

udp_receiver::udpReceiver::udpReceiver(const unsigned long int &port)
    : m_port(port), buffer(std::make_shared<std::vector<char>>(512, '0')) {
  sockaddr_in si_me;

  if ((m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
    throw configReaderException("Can't create socket");
  }

  memset((char *)&si_me, 0, sizeof(si_me));

  si_me.sin_family = AF_INET;
  si_me.sin_port = htons(port);
  si_me.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(m_socket, (struct sockaddr *)&si_me, sizeof(si_me)) == -1) {
    throw configReaderException("Can't bind socket to port");
  }
};

udp_receiver::udpReceiver::~udpReceiver() {
  close(m_socket);
};

std::shared_ptr<std::vector<char>> udp_receiver::udpReceiver::receiveData(long int &receive_length) {

  sockaddr_in si_other;
  int slen = sizeof(si_other);

  if ((receive_length = recvfrom(m_socket, buffer->data(), buffer->size(), 0, (struct sockaddr *)&si_other, (socklen_t *)&slen)) == -1) {
    throw configReaderException("Can't receive udp data");
  }

  if (sendto(m_socket, buffer->data(), receive_length, 0, (struct sockaddr *)&si_other, slen) == -1) {
    throw configReaderException("Can't send data to server");
  }

  return buffer;
};
