#ifndef ROCKET_NET_WAKEUP_FDEVENT_H
#define ROCKET_NET_WAKEUP_FDEVENT_H


#include "rocket/net/fd_event.h"

namespace rocket {

class WakeUpFdEvent : public FdEvent {
private:
  int m_fd;

public:
  WakeUpFdEvent(int fd);
  ~WakeUpFdEvent();

  void wakeup();
};

} // namespace rocket

#endif