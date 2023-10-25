#ifndef hsrpc_NET_TIMER_H
#define hsrpc_NET_TIMER_H
#include "hsrpc/common/mutex.h"
#include "hsrpc/net/fd_event.h"
#include "hsrpc/net/time_event.h"
#include <map>
namespace hsrpc {
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
} // namespace hsrpc

#endif