#include "rocket/net/tcp/tcp_server.h"
#include "rocket/common/log.h"
#include "rocket/net/eventloop.h"
namespace rocket {

TcpServer::TcpServer(NetAddr::s_ptr local_addr) : m_local_addr(local_addr) {
  m_acceptor = std::make_shared<TcpAcceptor>(m_local_addr);

  m_main_event_loop = Eventloop::GetCurrentEventLoop();
  m_io_thread_group = new IOThreadGroup(3);
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

  m_main_event_loop = Eventloop::GetCurrentEventLoop();
  m_io_thread_group = new IOThreadGroup(2);

  m_listen_fd_evnet = new FdEvent(m_acceptor->getListenFd());
  m_listen_fd_evnet->listen(FdEvent::IN_EVENT,
                            std::bind(TcpServer::onAccept, this));

  m_main_event_loop->addEvpollEvent(m_listen_fd_evnet);
}

void TcpServer::onAccept() { 
    
    
    int client_fd = m_acceptor->accept(); 

 
}

} // namespace rocket
