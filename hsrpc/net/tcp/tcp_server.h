#ifndef hsrpc_NET_TCP_SERVER_H
#define hsrpc_NET_TCP_SERVER_H
#include "hsrpc/net/io_threadgroup.h"
#include "hsrpc/net/tcp/net_addr.h"
#include "hsrpc/net/tcp/tcp_accept.h"
#include "hsrpc/net/tcp/tcp_connection.h"
namespace hsrpc {
class TcpServer {
public:
  TcpServer(NetAddr::s_ptr local_addr);
  ~TcpServer();

  void start();


private:
  void init();
  void onAccept();
private:
  TcpAcceptor::s_ptr m_acceptor;
  NetAddr::s_ptr m_local_addr;        // 本地监听地址
  Eventloop *m_main_event_loop{NULL}; // mainreactor
  IOThreadGroup *m_io_thread_group;   // subreactor

  FdEvent *m_listen_fd_evnet;
  int m_client_counts{0};

  std::set<TcpConnection::s_ptr> m_client;
};

} // namespace hsrpc

#endif