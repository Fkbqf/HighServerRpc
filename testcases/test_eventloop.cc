#include "rocket/common/config.h"
#include "rocket/common/log.h"
#include "rocket/net/eventloop.h"
#include "rocket/net/fd_event.h"
#include "rocket/net/io_thread.h"
#include "rocket/net/time_event.h"
#include <arpa/inet.h>
#include <memory>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include"rocket/net/io_threadgroup.h"
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
  sizeof(addr);
  if (rt != 0) {
    ERRORLOG("bind error");
    exit(1);
  }

  rt = listen(listenfd, 100);
  if (rt != 0) {
    ERRORLOG("listen error");
    exit(1);
  }

  rocket::FdEvent event(listenfd);
  event.listen(rocket::FdEvent::IN_EVENT, [listenfd]() {
    sockaddr_in peer_addr;
    socklen_t addr_len = sizeof(peer_addr);
    memset(&peer_addr, 0, sizeof(peer_addr));
    int clientfd =
        accept(listenfd, reinterpret_cast<sockaddr *>(&peer_addr), &addr_len);

    DEBUGLOG("success get client fd[%d], peer addr: [%s:%d]", clientfd,
        inet_ntoa(peer_addr.sin_addr), ntohs(peer_addr.sin_port));
  });

  int i = 0;
  rocket::TimerEvent::s_ptr timer_event = std::make_shared<rocket::TimerEvent>(
      1000, true, [&i]() { INFOLOG("trigger timer event, count=%d", i++); });

  // rocket::IoThtread io_thread;

  // io_thread.getEventloop()->addEvpollEvent(&event);
  // io_thread.getEventloop()->addTimerEvent(timer_event);
  // io_thread.start();

  // io_thread.join();

  rocket::IOThreadGroup io_thread_group(2);
  rocket::IoThtread* io_thread = io_thread_group.getIoThread();
  // io_thread->getEventLoop()->addEpollEvent(&event);
  io_thread->getEventloop()->addTimerEvent(timer_event);

  rocket::IoThtread* io_thread2 = io_thread_group.getIoThread();
  io_thread2->getEventloop()->addTimerEvent(timer_event);

  io_thread_group.start();

  io_thread_group.join();
}

int main() {

  rocket::Config::SetGlobalConfig(
      "/home/fyt/workespace/2work/T-ServerPc/conf/rocket.xml");

  rocket::Logger::InitGlobalLogger();
  test_io_thread();
  // rocket::Eventloop *eventloop = new rocket::Eventloop();

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

  // rocket::FdEvent event(listenfd);
  // event.listen(rocket::FdEvent::IN_EVENT, [listenfd]() {
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
  // // rocket::TimerEvent::s_ptr timer_event =
  // // std::make_shared<rocket::TimerEvent>(
  // //   1000, true, [&i]() {
  // //     INFOLOG("trigger timer event, count=%d", i++);
  // //   }
  // // );

  // // eventloop->addTimerEvent(timer_event);
  // eventloop->loop();

  return 0;
}