#ifndef hsrpc_NET_TCP_NET_ADDR_H
#define hsrpc_NET_TCP_NET_ADDR_H

#include <arpa/inet.h>
#include <memory>
#include <string.h>
#include <string>
namespace hsrpc {
class NetAddr {

public:
  typedef std::shared_ptr<NetAddr> s_ptr;

  virtual sockaddr *getSockAddr() = 0;

  virtual socklen_t getSockLen() = 0;

  virtual int getFamily() = 0;

  virtual std::string toString() = 0;

  virtual bool checkValid() = 0;
  
};

class IpNetAddr : public NetAddr {
public:
  IpNetAddr(const std::string& ip,uint16_t port);
  IpNetAddr(const std::string& addr);
  IpNetAddr(sockaddr_in addr);
  ~IpNetAddr();

  sockaddr *getSockAddr();

  socklen_t getSockLen();

  int getFamily();

  std::string toString();

  bool checkValid();

private:
  std::string m_ip;
  uint16_t m_port{0};
  sockaddr_in m_addr;
};

} // namespace hsrpc

#endif