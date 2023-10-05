#include "eventloop.h"
#include "rocket/common/log.h"
#include "rocket/common/util.h"
#include <cerrno>
#include <cstddef>
#include <cstdlib>
#include <functional>
#include <queue>
#include <string.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <sys/socket.h>

#define ADD_TO_EPOLL()                                                         \
  auto it = m_listen_fds.find(event->getFd());                                 \
  int op = EPOLL_CTL_ADD;                                                      \
  if (it != m_listen_fds.end()) {                                              \
    op = EPOLL_CTL_MOD;                                                        \
  }                                                                            \
  epoll_event tmp = event->getEpollEvent();                                    \
  int rt = epoll_ctl(m_epoll_fd, op, event->getFd(), &tmp);                    \
  if (rt == -1) {                                                              \
    ERRORLOG("failed epoll_ctl when add fd,errono=%d,error=%s", errno,         \
             strerror(errno))                                                  \
  }                                                                            \
  DEBUGLOG("add event success,fd[%d]", event->getFd())

#define DELETE_TO_EPOLL()                                                      \
  auto it = m_listen_fds.find(event->getFd());                                 \
  if (it != m_listen_fds.end()) {                                              \
    return;                                                                    \
  }                                                                            \
  int op = EPOLL_CTL_DEL;                                                      \
  epoll_event tmp = event->getEpollEvent();                                    \
  int rt = epoll_ctl(m_epoll_fd, op, event->getFd(), &tmp);                    \
  if (rt == -1) {                                                              \
    ERRORLOG("failed epoll_ctl when add fd,errono=%d,error=%s", errno,         \
             strerror(errno))                                                  \
  }                                                                            \
  DEBUGLOG("del event success fd[%d]", event->getFd());

namespace rocket {

static thread_local Eventloop *t_current_eventloop = NULL;
static int g_epool_max_timeout = 10000;
static int g_epool_max_events = 10;

Eventloop::Eventloop() {
  if (t_current_eventloop != NULL) {
    ERRORLOG(" failed to creat eventloop,this thread has created event loop")
    exit(0);
  }
  m_thread_pid = getThreadId();

  m_epoll_fd = epoll_create(10);
  if (m_epoll_fd == -1) {
    ERRORLOG("failed to creat event loop,epoll_create err, error info [%d]",
             errno)
    exit(0);
  }

  initwakeupfdevent();

  INFOLOG("succ create event loop int thread %d", m_thread_pid)

  t_current_eventloop = this;
}

Eventloop::~Eventloop() {
  close(m_epoll_fd);
  if (m_wakeup_fd_event) {
    delete m_wakeup_fd_event;
    m_wakeup_fd_event = NULL;
  }
}

void Eventloop::initwakeupfdevent() {
  m_wakeup_fd = eventfd(0, EFD_NONBLOCK);
  if (m_wakeup_fd < 0) {
    ERRORLOG("failed to eventfd , error info [%d]", errno)
    exit(0);
  }
  m_wakeup_fd_event = new WakeUpFdEvent(m_wakeup_fd);

  m_wakeup_fd_event->listen(FdEvent::IN_EVENT, [this]() {
    char buf[8];
    while (read(m_wakeup_fd, buf, 8) != -1 && errno != EAGAIN) {
    };
    DEBUGLOG("read full bytes from wakup fd[%d]", m_wakeup_fd);
  });

  addEvpollEvent(m_wakeup_fd_event);
}

void Eventloop::loop() {
  while (!m_stop_flag) {
    // 先遍历任务，防止有剩余任务
    ScopeMutext<Mutex> lock(m_mutex);
    std::queue<std::function<void()>> tmp_tasks ;
    m_pending_tasks.swap(tmp_tasks);
    lock.unlock();

    while (!tmp_tasks.empty()) {
      auto cb = tmp_tasks.front();
      tmp_tasks.pop();

      if (cb) {
        cb();
      }
    }

    int timeout = g_epool_max_timeout;

    epoll_event result_events[g_epool_max_events];

    DEBUGLOG("epooll wait start");
    int rt = epoll_wait(m_epoll_fd, result_events, g_epool_max_events, timeout);
    DEBUGLOG("epooll wait end");
    if (rt < 0) {
      ERRORLOG("epoll_wait error ,errno=%d", errno)
    } else {
      for (int i = 0; i < rt; ++i) {
        epoll_event trigger_event = result_events[i];
        FdEvent *fd_event = static_cast<FdEvent *>(trigger_event.data.ptr);
        if (fd_event == NULL) {
          continue;
        }

        if (trigger_event.events & EPOLLIN) {
          addTask(fd_event->handler(FdEvent::IN_EVENT));
        }

        if (trigger_event.events & EPOLLOUT) {
          addTask(fd_event->handler(FdEvent::IN_EVENT));
        }
      }
    }
  }
}

void Eventloop::addEvpollEvent(FdEvent *event) {
  if (isInloopthread()) {
    ADD_TO_EPOLL()
  } else {
    auto cb = [this, event]() { ADD_TO_EPOLL(); };
    addTask(cb, true);
  }
}

void Eventloop::delEvpollEvent(FdEvent *event) {

  if (isInloopthread()) {
    DELETE_TO_EPOLL();
  } else {
    auto cb = [this, event]() { DELETE_TO_EPOLL(); };
    addTask(cb, true);
  }
}

void Eventloop::addTask(std::function<void()> cb, bool is_wake_up /*false*/) {
  ScopeMutext<Mutex> lock(m_mutex);
  m_pending_tasks.push(cb);
  lock.unlock();
  if (is_wake_up) {
    wakeup();
  }
}
void Eventloop::wakeup() {
  INFOLOG("WAKE UP");

  m_wakeup_fd_event->wakeup();
}

void Eventloop::stop() { m_stop_flag = true; }

void Eventloop::dealwakup() {}
bool Eventloop::isInloopthread() { return getThreadId() == m_thread_pid; }
} // namespace rocket
