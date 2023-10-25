#ifndef hsrpc_NET_IO_THREAD_GROUP_H
#define hsrpc_NET_IO_THREAD_GROUP_H
#include "hsrpc/common/log.h"
#include "hsrpc/net/io_thread.h"
#include <vector>
namespace hsrpc {
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

} // namespace hsrpc

#endif