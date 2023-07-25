#include "eventloop.h"
#include "../common/log.h"
#include "../common/util.h"
#include <cerrno>
#include <cstddef>
#include <cstdlib>
#include <functional>
#include <queue>
#include <sys/epoll.h>
#include <sys/socket.h>
namespace rocket {

static thread_local Eventloop *t_current_eventloop = NULL;
static int g_epool_max_timeout = 10000;
static int g_epool_max_events = 10;
Eventloop::Eventloop() {
  if (t_current_eventloop != NULL) {
    ERRORLOG("failed to creat eventloop,this thread has created event loop")
    exit(0);
  }
  m_pid = getThreadId();

  m_epoll_fd = epoll_create(10);
  if (m_epoll_fd == -1) {
    ERRORLOG("failed to creat event loop,epoll_create err, error info [%d]",
             errno)
    exit(0);
  }

  INFOLOG("succ create event loop int thread %d", m_pid)

  t_current_eventloop = this;
}
Eventloop::~Eventloop() {}
void Eventloop::loop() {
  while (!m_stop_flag) {
    // 先便利任务，防止有剩余任务
    ScopeMutext<Mutex> lock(m_mutex);
    std::queue<std::function<void()>> tmp_tasks = m_pending_tasks;
    m_pending_tasks.swap(tmp_tasks);
    lock.unlock();

    while (!tmp_tasks.empty()) {
      tmp_tasks.front()();
      tmp_tasks.pop();
    }

    int timeout = g_epool_max_timeout;

    epoll_event result_events[g_epool_max_events];

    int rt = epoll_wait(m_epoll_fd, result_events, g_epool_max_events, timeout);

    if (rt < 0) {
      ERRORLOG("epoll_wait error ,errno=%d", errno)
    } else {
      for (int i = 0; i < rt; ++i) {
        epoll_event trigger_event = result_events[i];
      }
    }
  }
}
void Eventloop::wakeup() {}
void Eventloop::stop() {}

void Eventloop::dealwakup()[

]
} // namespace rocket
