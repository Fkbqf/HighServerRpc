#ifndef ROCKET_NET_IO_THREAD_GROUP_H
#define ROCKET_NET_IO_THREAD_GROUP_H
#include "rocket/common/log.h"
#include "rocket/net/io_thread.h"
#include <vector>
namespace rocket {
class IOThreadGroup {
private:
  int m_size{0};
  int m_index{0};
  std::vector<IoThtread *> m_io_thread_groups;

public:
  IOThreadGroup(int size);
  ~IOThreadGroup();
  void start();
  void join();
  IoThtread *getIoThread();
};

} // namespace rocket

#endif