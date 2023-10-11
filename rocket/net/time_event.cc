#include "time_event.h"
#include "rocket/common/log.h"
#include "rocket/common/util.h"
namespace rocket {

TimerEvent::TimerEvent(int interval, bool is_repeated, std::function<void()> cb)
    : m_interval(interval), m_is_repeated(is_repeated), m_task(cb) {
  resetArriveTime();
}
TimerEvent::~TimerEvent(){}

void TimerEvent::resetArriveTime() {
  m_arrive_time = getNoMs() + m_interval;
  DEBUGLOG("success create timer event , will execut at[%lld]", m_arrive_time);
}
} // namespace rocket