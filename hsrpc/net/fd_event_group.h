#ifndef hsrpc_NET_FD_EVENT_GROUP_H
#define hsrpc_NET_FD_EVENT_GROUP_H

#include <hsrpc/net/fd_event.h>

#include "hsrpc/common/mutex.h"
#include <vector>

namespace hsrpc {
class FdEventGroup {
public:
  FdEventGroup();
  FdEventGroup(int size);
  ~FdEventGroup();

  FdEvent *getFdEvent(int fd);

public:
  static FdEventGroup *GetFdEventGroup();

private:
  int m_size = 0;
  std::vector<FdEvent *> m_fd_group;
  Mutex m_mutex;
};

} // namespace hsrpc

#endif