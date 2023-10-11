#include "rocket/common/log.h"
#include "rocket/net/tcp/net_addr.h"

int main() {

    rocket::Config::SetGlobalConfig(
      "/home/fyt/workespace/2work/T-ServerPc/conf/rocket.xml");

  rocket::Logger::InitGlobalLogger();
  rocket::IpNetAddr addr("127.0.0.1:12345");

DEBUGLOG("ip:port %s ",addr.toString().c_str());

}