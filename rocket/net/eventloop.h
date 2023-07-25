#ifndef ROCKET_NET_EVENTLOOP_H
#define ROCKET_NET_EVENTLOOP_H

#include "../common/mutex.h"
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

private:
  void dealwakup();

private:
  pid_t m_pid;
  std::set<int> m_listen_fds;
  int m_epoll_fd{0};
  std::queue<std::function<void()>> m_pending_tasks;
  bool m_stop_flag{false};
  int m_wakeup_fd{0};
  Mutex m_mutex;
};

} // namespace rocket

#endif