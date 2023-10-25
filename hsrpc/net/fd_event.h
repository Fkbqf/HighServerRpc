#ifndef ROCK_NET_FDEVENT_H
#define ROCK_NET_FDEVENT_H

#include <functional>
#include <sys/epoll.h>
// FdEvent 类
namespace hsrpc {

class FdEvent {
public:
  // 枚举类型，表示可能的触发事件
  enum TriggerEvent {
    IN_EVENT = EPOLLIN,    // 可读事件
    OUT_EVENT = EPOLLOUT,  // 可写事件
    ERROR_EVENT = EPOLLERR // 错误事件
  };

  // 构造函数，参数为一个文件描述符
  FdEvent(int fd);

  // 默认构造函数
  FdEvent();

  // 析构函数
  ~FdEvent();

  // 设置文件描述符为非阻塞模式
  void setNonBlock();

  // 根据事件类型返回对应的处理函数
  std::function<void()> handler(TriggerEvent event_type);

  // 监听指定类型的事件，如果事件发生，就调用对应的回调函数
  void listen(TriggerEvent event_type, std::function<void()> callback,
              std::function<void()> err_callback = nullptr);

  // 取消监听指定类型的事件
  void cancel(TriggerEvent event_type);

  // 返回文件描述符
  int getFd() const { return m_fd; }

  // 返回 epoll_event 结构
  epoll_event getEpollEvent() { return m_listen_events; }


protected:
  // 文件描述符
  int m_fd{-1};

  // 监听的事件
  epoll_event m_listen_events;

  // 可读事件的回调函数
  std::function<void()> m_read_callback{nullptr};

  // 可写事件的回调函数
  std::function<void()> m_write_callback{nullptr};

  // 错误事件的回调函数
  std::function<void()> m_error_callback{nullptr};
};

} // namespace hsrpc

#endif