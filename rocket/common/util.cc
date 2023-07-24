#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include "rocket/common/util.h"

/*
在这段代码中，static 关键字的使用有两个目的：

static 修饰的全局变量 g_pid：在 C++ 中，static 关键字用于全局变量或函数时，会限制其可见性只在定义它的文件内。这意味着，g_pid 只在定义它的 .cpp 文件中可见，不能在其他文件中直接访问。这样做可以防止全局变量名的冲突，同时有助于维护代码的模块化。

static thread_local 修饰的变量 g_thread_id：thread_local 表示该变量是线程局部的，也就是每个线程都有一份该变量的副本，各个线程之间互不干扰。而 static 的作用在于，确保 g_thread_id 在每个线程的生命周期内只被初始化一次。如果没有 static，那么每次通过 getThreadId 访问 g_thread_id 时，都会重新进行初始化。
*/
namespace rocket {

static int g_pid = 0; //进程号是多个线程共有的

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

}