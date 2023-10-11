#ifndef ROCKET_NET_TCP_TCP_CONNECTION_H
#define ROCKET_NET_TCP_TCP_CONNECTION_H
#include "rocket/net/io_thread.h"
#include "rocket/net/tcp/net_addr.h"
#include "rocket/net/tcp/tcp_buffer.h"
namespace rocket {
class TcpConnection {
public:
  enum TcpState {
    NotConnected = 1,
    Conected = 2,
    HalfClosing = 3,
    Closed = 4,
  };

public:
  TcpConnection(IoThtread *io_thread, int fd, int buff_size,
                NetAddr::s_ptr peer_add);
  ~TcpConnection();

  void onRead();
  void excute();
  void onWrite();

private:
  NetAddr::s_ptr m_local_addr;
  NetAddr::s_ptr m_peer_addr;

  TcpBuffer::s_ptr m_in_buffer;  // 接收缓冲区
  TcpBuffer::s_ptr m_out_buffer; // 发送缓冲区

  IoThtread *m_io_thread{NULL};
  FdEvent *m_fd_event{NULL};

  TcpState m_state;
    int m_fd {0};
};

} // namespace rocket

#endif