#ifndef hsrpc_NET_WAKEUP_FDEVENT_H
#define hsrpc_NET_WAKEUP_FDEVENT_H


#include "hsrpc/net/fd_event.h"

namespace hsrpc {

class WakeUpFdEvent : public FdEvent {

public:
  WakeUpFdEvent(int fd);
  ~WakeUpFdEvent();

  void wakeup();
};

} // namespace hsrpc

#endif