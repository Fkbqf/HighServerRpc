#ifndef ROCKET_NET_TCP_SERVER_H
#define ROCKET_NET_TCP_SERVER_H
#include "rocket/net/io_threadgroup.h"
#include "rocket/net/tcp/net_addr.h"
#include "rocket/net/tcp/tcp_accept.h"
namespace rocket {
class TcpServer {
public:
  TcpServer(NetAddr::s_ptr local_addr);
  ~TcpServer();

  void start();
  void onAccept();

private:
  void init();

private:
  TcpAcceptor::s_ptr m_acceptor;
  NetAddr::s_ptr m_local_addr;        // 本地监听地址
  Eventloop *m_main_event_loop{NULL}; // mainreactor
  IOThreadGroup *m_io_thread_group;   // subreactor

  FdEvent *m_listen_fd_evnet;
  int m_client_counts{0};
};

} // namespace rocket

#endif