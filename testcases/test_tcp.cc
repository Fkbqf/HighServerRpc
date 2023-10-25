#include <memory>
#include "hsrpc/common/log.h"
#include "hsrpc/net/tcp/net_addr.h"
#include "hsrpc/net/tcp/tcp_server.h"

void test_tcp_server() {

  hsrpc::IpNetAddr::s_ptr addr = std::make_shared<hsrpc::IpNetAddr>("127.0.0.1", 12346);

  DEBUGLOG("create addr %s", addr->toString().c_str());

  hsrpc::TcpServer tcp_server(addr);

  tcp_server.start();

}

int main() {

  hsrpc::Config::SetGlobalConfig("../conf/hsrpc.xml");
  hsrpc::Logger::InitGlobalLogger();

  test_tcp_server();
  
}