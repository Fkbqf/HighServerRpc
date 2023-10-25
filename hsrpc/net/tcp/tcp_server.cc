#include "hsrpc/net/tcp/tcp_server.h"
#include "hsrpc/common/log.h"
#include "hsrpc/net/eventloop.h"
#include "hsrpc/net/tcp/tcp_connection.h"
namespace hsrpc {

TcpServer::TcpServer(NetAddr::s_ptr local_addr) : m_local_addr(local_addr) {
  init();
  INFOLOG("hsrpc TcpServer listen sucess on [%s]", m_local_addr->toString().c_str());

}

TcpServer::~TcpServer() {
  if (m_main_event_loop) {
    delete m_main_event_loop;
    m_main_event_loop = NULL;
  }
}

void TcpServer::start() {
  m_io_thread_group->start();
  m_main_event_loop->loop();
}

void TcpServer::init() {

  m_acceptor = std::make_shared<TcpAcceptor>(m_local_addr);
   
  m_main_event_loop = Eventloop::GetCurrentEventloop();
  m_io_thread_group = new IOThreadGroup(2);

  m_listen_fd_evnet = new FdEvent(m_acceptor->getListenFd());
  m_listen_fd_evnet->listen(FdEvent::IN_EVENT,
                            std::bind(&TcpServer::onAccept, this));

  m_main_event_loop->addEpollEvent(m_listen_fd_evnet);
}

void TcpServer::onAccept() {
  auto re = m_acceptor->accept();
  int client_fd = re.first;
  NetAddr::s_ptr peer_addr = re.second;

  m_client_counts++;

  // 把clientfd添加到任意的io线程里面

  IoThtread *io_thread = m_io_thread_group->getIoThread();
  TcpConnection::s_ptr connetion = std::make_shared<TcpConnection>(
      io_thread->getEventloop(), client_fd, 128,m_local_addr, peer_addr);
  connetion->setState(Connected);

  m_client.insert(connetion);

  INFOLOG("TcpServer succ get client, fd=%d", client_fd);
}

} // namespace hsrpc
