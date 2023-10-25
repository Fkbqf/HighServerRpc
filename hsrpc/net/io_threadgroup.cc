#include "io_threadgroup.h"
namespace hsrpc {

IOThreadGroup::IOThreadGroup(int size) : m_size(size) {
  m_io_thread_groups.resize(size);
  for (size_t i = 0; (int)i < size; ++i) {
    m_io_thread_groups[i] = new IoThtread();
  }
}

IOThreadGroup::~IOThreadGroup() {

}

void IOThreadGroup::start() {
  for (size_t i = 0; i < m_io_thread_groups.size(); ++i) {
    m_io_thread_groups[i]->start();
  }
}

void IOThreadGroup::join() {
  for (size_t i = 0; i < m_io_thread_groups.size(); ++i) {
    m_io_thread_groups[i]->join();
  }
} 

IoThtread* IOThreadGroup::getIoThread() {
  if (m_index == (int)m_io_thread_groups.size() || m_index == -1)  {
    m_index = 0;
  }
  return m_io_thread_groups[m_index++];
}

} // namespace hsrpc