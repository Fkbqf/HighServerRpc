#ifndef ROCKET_NET_IO_THREAD_H
#define ROCKET_NET_IO_THREAD_H

#include "rocket/net/eventloop.h"
#include <pthread.h>
#include <semaphore.h>
namespace rocket {
class IoThtread {
private:
  pid_t m_thread_id{-1};  // 线程号
  pthread_t m_thread{0}; // 线程句柄
 
  Eventloop *m_event_loop{NULL};
 
  sem_t m_init_semapthore;
  sem_t m_start_semaphore;
public:
  static void *Main(void *arg);

  void join();
  Eventloop *getEventloop();
  void start();
  IoThtread();
  ~IoThtread();
};

} // namespace rocket

#endif