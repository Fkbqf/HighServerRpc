#include "rocket/net/tcp/tcp_connection.h"
#include "rocket/common/log.h"
#include "rocket/net/fd_event_group.h"
#include <unistd.h>
namespace rocket {
TcpConnection::TcpConnection(IoThtread *io_thread, int fd, int buff_size,
                             NetAddr::s_ptr peer_addr)
    : m_io_thread(io_thread), m_peer_addr(peer_addr), m_state(NotConnected) {

  m_in_buffer = std::make_shared<TcpBuffer>(buff_size);
  m_out_buffer = std::make_shared<TcpBuffer>(buff_size);
  m_fd_event = FdEventGroup::GetFdEventGroup()->getFdEvent(fd);
  m_fd_event->listen(FdEvent::IN_EVENT,
                     std::bind(&TcpConnection::onRead, this));
}

TcpConnection::~TcpConnection() {
  // 1. 从socket缓冲区 调用系统read函数读取字节in_buffer 里面

  if (m_state != Conected) {
    INFOLOG("client has already disconnected");
    return;
  }
}

void TcpConnection::onRead() {
  if (m_state != Conected) {
    INFOLOG("client has already disconnected,addr [%s],clientfd [%d]",
            m_peer_addr->toString().c_str(), m_fd_event->getFd());
    return;
  }

  bool is_read_all = false;
  bool is_close = false;
  while (!is_read_all) {
    if (m_in_buffer->writeAble() == 0) {
      m_in_buffer->resizeBuffer(2 * m_in_buffer->m_buffer.size());
    }

    int read_count = m_in_buffer->writeAble();
    int write_index = m_in_buffer->writerIndex();

    int rt = read(m_fd_event->getFd(), &(m_in_buffer->m_buffer[write_index]),
                  read_count);
    DEBUGLOG("success read %d bytes from addr[%s],client fd[%d]", rt,
             m_peer_addr->toString().c_str(), m_fd)
    if (rt > 0) {
      m_in_buffer->moveWriteIndex(rt);

      if (rt == read_count) {
        continue;
      } else if (rt < read_count) {
        is_read_all = true;
        break;
      }

    } else {
      is_close = true;
    }
  }

  if (is_close) {
    // TODO:处理关闭链接
    INFOLOG("peer closed , peer addr[%d] , clientfd[%d]",
            m_peer_addr->toString().c_str(), m_fd);
  }

  if (!is_read_all) {
    ERRORLOG("not read all data");
  }
  // TODO:简单的echo,补充rpc解析
  excute();
}
void TcpConnection::excute() {
  // 将rpc请求执行业务逻辑，获取rpc响应，再把rpc响应发回去
  std::vector<char> tmp;
  int size = m_in_buffer->readAble();
  tmp.resize(size);
  m_in_buffer->readFromBuffer(tmp, size);

  INFOLOG("success get request from clien[%s] ",
          m_peer_addr->toString().c_str());
  m_fd_event->listen(FdEvent::OUT_EVENT,
                     std::bind(&TcpConnection::onWrite, this));
}
void TcpConnection::onWrite() {}
} // namespace rocket