#ifndef ROCKET_NET_TIMER_H
#define ROCKET_NET_TIMER_H
#include "rocket/common/mutex.h"
#include "rocket/net/fd_event.h"
#include "rocket/net/time_event.h"
#include <map>
namespace rocket {
class Timer : public FdEvent {
private:
  std::multimap<int64_t, TimerEvent::s_ptr> m_pending_events;
  Mutex m_mutex;

private:
   void resetArriveTime();

public:
  Timer();
  ~Timer();

  void addTimerEvent(TimerEvent::s_ptr event);
  void delTimerEvent(TimerEvent::s_ptr event);
  void onTimer();
};
} // namespace rocket

#endif