#include "hsrpc/common/config.h"
#include "hsrpc/common/log.h"
#include "hsrpc/net/eventloop.h"
#include "hsrpc/net/fd_event.h"
#include "hsrpc/net/io_thread.h"
#include "hsrpc/net/time_event.h"
#include <arpa/inet.h>
#include <memory>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include"hsrpc/net/io_threadgroup.h"
void test_io_thread() {

  int listenfd = socket(AF_INET, SOCK_STREAM, 0);
  if (listenfd == -1) {
    ERRORLOG("listenfd = -1");
    exit(0);
  }

  sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));

  addr.sin_port = htons(12310);
  addr.sin_family = AF_INET;
  inet_aton("127.0.0.1", &addr.sin_addr);
  socklen_t  size=sizeof(addr);
  int rt = bind(listenfd, reinterpret_cast<sockaddr *>(&addr),size);
  if (rt != 0) {
    ERRORLOG("bind error");
    exit(1);
  }

  rt = listen(listenfd, 100);
  if (rt != 0) {
    ERRORLOG("listen error");
    exit(1);
  }

  hsrpc::FdEvent event(listenfd);
  event.listen(hsrpc::FdEvent::IN_EVENT, [listenfd]() {
    sockaddr_in peer_addr;
    socklen_t addr_len = sizeof(peer_addr);
    memset(&peer_addr, 0, sizeof(peer_addr));
    int clientfd =
        accept(listenfd, reinterpret_cast<sockaddr *>(&peer_addr), &addr_len);

    DEBUGLOG("success get client fd[%d], peer addr: [%s:%d]", clientfd,
        inet_ntoa(peer_addr.sin_addr), ntohs(peer_addr.sin_port));
  });

  int i = 0;
  hsrpc::TimerEvent::s_ptr timer_event = std::make_shared<hsrpc::TimerEvent>(
      1000, true, [&i]() { INFOLOG("trigger timer event, count=%d", i++); });

  // hsrpc::IoThtread io_thread;

  // io_thread.getEventloop()->addEvpollEvent(&event);
  // io_thread.getEventloop()->addTimerEvent(timer_event);
  // io_thread.start();

  // io_thread.join();

  hsrpc::IOThreadGroup io_thread_group(2);
  hsrpc::IoThtread* io_thread = io_thread_group.getIoThread();
  // io_thread->getEventLoop()->addEpollEvent(&event);
  io_thread->getEventloop()->addTimerEvent(timer_event);

  hsrpc::IoThtread* io_thread2 = io_thread_group.getIoThread();
  io_thread2->getEventloop()->addTimerEvent(timer_event);

  io_thread_group.start();

  io_thread_group.join();
}

int main() {

  hsrpc::Config::SetGlobalConfig(
      "/home/fyt/workespace/2work/T-ServerPc/conf/hsrpc.xml");

  hsrpc::Logger::InitGlobalLogger();
  test_io_thread();
  // hsrpc::Eventloop *eventloop = new hsrpc::Eventloop();

  // int listenfd = socket(AF_INET, SOCK_STREAM, 0);
  // if (listenfd == -1) {
  //   ERRORLOG("listenfd = -1");
  //   exit(0);
  // }

  // sockaddr_in addr;
  // memset(&addr, 0, sizeof(addr));

  // addr.sin_port = htons(12310);
  // addr.sin_family = AF_INET;
  // inet_aton("127.0.0.1", &addr.sin_addr);

  // int rt = bind(listenfd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr));
  // if (rt != 0) {
  //   ERRORLOG("bind error");
  //   exit(1);
  // }

  // rt = listen(listenfd, 100);
  // if (rt != 0) {
  //   ERRORLOG("listen error");
  //   exit(1);
  // }

  // hsrpc::FdEvent event(listenfd);
  // event.listen(hsrpc::FdEvent::IN_EVENT, [listenfd]() {
  //   sockaddr_in peer_addr;
  //   socklen_t addr_len = sizeof(peer_addr);
  //   memset(&peer_addr, 0, sizeof(peer_addr));
  //   int clientfd =
  //       accept(listenfd, reinterpret_cast<sockaddr *>(&peer_addr),
  //       &addr_len);

  //   DEBUGLOG("success get client fd[%d], peer addr: [%s:%d]", clientfd,
  //            inet_ntoa(peer_addr.sin_addr), ntohs(peer_addr.sin_port));
  // });
  // eventloop->addEvpollEvent(&event);

  // // int i = 0;
  // // hsrpc::TimerEvent::s_ptr timer_event =
  // // std::make_shared<hsrpc::TimerEvent>(
  // //   1000, true, [&i]() {
  // //     INFOLOG("trigger timer event, count=%d", i++);
  // //   }
  // // );

  // // eventloop->addTimerEvent(timer_event);
  // eventloop->loop();

  return 0;
}