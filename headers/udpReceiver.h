#ifndef __UDPRECEIVER_H__
#define __UDPRECEIVER_H__

#include <memory>
#include <vector>

namespace udp_receiver {

  class udpReceiver {
  private:
    unsigned long int m_port;
    int m_socket;
    std::shared_ptr<std::vector<char>> buffer;

  public:
    udpReceiver(const unsigned long int &port);

    ~udpReceiver();

    std::shared_ptr<std::vector<char>> receiveData(long int &receive_length);
  };

} // namespace udp_receiver

#endif //__UDPRECEIVER_H__
