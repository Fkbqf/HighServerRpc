#include "rocket/net/io_thread.h"
#include "rocket/common/log.h"
#include "rocket/common/util.h"
#include <assert.h>
#include <pthread.h>
namespace rocket {

IoThtread::IoThtread() {

  int rt = sem_init(&m_init_semapthore, 0, 0);
  assert(rt == 0);

  rt = sem_init(&m_start_semaphore, 0, 0);
  assert(rt == 0);

  rt = pthread_create(&m_thread, NULL, &IoThtread::Main, this);

  // wait 直到新函数执行完main函数的前置
  sem_wait(&m_init_semapthore);
  DEBUGLOG("Iothread [%d] create success", m_thread_id);
}

IoThtread::~IoThtread() {
  m_event_loop->stop();

  sem_destroy(&m_init_semapthore);
  sem_destroy(&m_start_semaphore);
  pthread_join(m_thread, NULL);
  if (m_event_loop) {
    delete m_event_loop;
    m_event_loop = NULL;
  }
}

void *IoThtread::Main(void *arg) {
  IoThtread *thread = static_cast<IoThtread *>(arg);
  thread->m_event_loop = new Eventloop();
  thread->m_thread_id = getThreadId();

  // 唤醒等待线程
  sem_post(&thread->m_init_semapthore);
  DEBUGLOG("IOThread %d created, wait start semaphore", thread->m_thread_id);

  sem_wait(&thread->m_start_semaphore);
  DEBUGLOG("IOThread %d start loop ", thread->m_thread_id);

  thread->m_event_loop->loop();
  DEBUGLOG("IOThread %d end loop ", thread->m_thread_id);

  return NULL;
}
void IoThtread::start() {
  DEBUGLOG("Now invoke IOThread %d", m_thread_id);
  sem_post(&m_start_semaphore);
}
void IoThtread::join() { pthread_join(m_thread, NULL); }
Eventloop *IoThtread::getEventloop() { return m_event_loop; }
} // namespace rocket