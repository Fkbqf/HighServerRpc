#ifndef ROCKET_NET_FD_EVENT_GROUP_H
#define ROCKET_NET_FD_EVENT_GROUP_H

#include <rocket/net/fd_event.h>

#include "rocket/common/mutex.h"
#include <vector>

namespace rocket {
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

} // namespace rocket

#endif