#ifndef ROCKET_NET_EVENTLOOP_H
#define ROCKET_NET_EVENTLOOP_H
#include "rocket/net/wakeup_fd_event.h"

#include "fd_event.h"
#include "rocket/common/mutex.h"
#include <functional>
#include <pthread.h>
#include <queue>
#include <set>

namespace rocket {

class Eventloop {
public:
  Eventloop();

  ~Eventloop();

  void loop();

  void wakeup();

  void stop();

  void addEvpollEvent(FdEvent *event);

  void delEvpollEvent(FdEvent *event);

  bool isInloopthread();

  void addTask(std::function<void()> cb, bool is_wake_up = false);

private:
  void dealwakup();
  void initwakeupfdevent();

private:
  pid_t m_thread_pid{0};

  std::set<int> m_listen_fds;

  int m_epoll_fd{0};

  WakeUpFdEvent *m_wakeup_fd_event;

  std::queue<std::function<void()>> m_pending_tasks;

  bool m_stop_flag{false};

  int m_wakeup_fd{0};

  Mutex m_mutex;
};

} // namespace rocket

#endif