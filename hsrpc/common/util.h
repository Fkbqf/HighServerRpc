#ifndef hsrpc_COMMON_UTIL_H
#define hsrpc_COMMON_UTIL_H

#include <sys/types.h>
#include <unistd.h>

namespace hsrpc {

pid_t getPid();

pid_t getThreadId();

int64_t getNoMs();

int32_t getInt32FromNetByte(const char* buf);

}

#endif