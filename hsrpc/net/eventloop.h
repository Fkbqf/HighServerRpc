#ifndef hsrpc_NET_Eventloop_H
#define hsrpc_NET_Eventloop_H

#include <pthread.h>
#include <set>
#include <functional>
#include <queue>
#include "hsrpc/common/mutex.h"
#include "hsrpc/net/fd_event.h"
#include "hsrpc/net/wakeup_fd_event.h"
#include "hsrpc/net/timer.h"

namespace hsrpc {
class Eventloop {
 public:
  Eventloop();

  ~Eventloop();

  void loop();

  void wakeup();

  void stop();

  void addEpollEvent(FdEvent* event);

  void deleteEpollEvent(FdEvent* event);

  bool isInLoopThread();

  void addTask(std::function<void()> cb, bool is_wake_up = false);

  void addTimerEvent(TimerEvent::s_ptr event);

  bool isLooping();

 public:
  static Eventloop* GetCurrentEventloop();


 private:
  void dealWakeup();

  void initWakeUpFdEevent();

  void initTimer();

 private:
  pid_t m_thread_id {0};

  int m_epoll_fd {0};

  int m_wakeup_fd {0};

  WakeUpFdEvent* m_wakeup_fd_event {NULL};

  bool m_stop_flag {false};

  std::set<int> m_listen_fds;

  std::queue<std::function<void()>> m_pending_tasks;
  
  Mutex m_mutex;

  Timer* m_timer {NULL};

  bool m_is_looping {false};

};

}


#endif