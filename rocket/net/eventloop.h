#ifndef ROCKET_NET_EVENTLOOP_H
#define ROCKET_NET_EVENTLOOP_H
#include "fd_event.h"
#include "rocket/common/mutex.h"
#include "rocket/net/timer.h"
#include "rocket/net/wakeup_fd_event.h"
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
  void addTimerEvent(TimerEvent::s_ptr event);

public:
  static Eventloop *GetCurrentEventLoop();

private:
  void dealwakup();
  void initwakeupfdevent();
  void initTimer();

private:
  pid_t m_thread_pid{0};

  int m_epoll_fd{0};
  int m_wakeup_fd{0};

  std::set<int> m_listen_fds;
  WakeUpFdEvent *m_wakeup_fd_event{NULL};

  std::queue<std::function<void()>> m_pending_tasks;

  bool m_stop_flag{false};

  Timer *m_timer{NULL};
  Mutex m_mutex;
  bool m_is_looping{false};
};

} // namespace rocket

#endif