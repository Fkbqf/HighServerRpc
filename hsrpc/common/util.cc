#include "hsrpc/common/util.h"
#include <arpa/inet.h>
#include <memory.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h> 
#include <string.h>
namespace hsrpc {

static int g_pid = 0;

static thread_local int g_thread_id = 0;

pid_t getPid() {
  if (g_pid != 0) {
    return g_pid;
  }
  return getpid();
}

pid_t getThreadId() {
  if (g_thread_id != 0) {
    return g_thread_id;
  }
  return syscall(SYS_gettid);
}

int64_t getNoMs() {
  timeval val;
  gettimeofday(&val, NULL);

  return val.tv_sec * 1000 + val.tv_usec / 1000;
}

int32_t getInt32FromNetByte(const char *buf) {
  int32_t re;
  memcpy(&re, buf, sizeof(re));
  return ntohl(re);
}
} // namespace hsrpc