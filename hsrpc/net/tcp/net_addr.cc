#include "net_addr.h"
#include "hsrpc/common/log.h"
namespace hsrpc {

IpNetAddr::IpNetAddr(const std::string &ip, uint16_t port)
    : m_ip(ip), m_port(port) {
  memset(&m_addr, 0, sizeof(m_addr));
  m_addr.sin_family = AF_INET;
  m_addr.sin_addr.s_addr = inet_addr(m_ip.c_str());
  m_addr.sin_port = htons(m_port);
}

IpNetAddr::IpNetAddr(const std::string &addr) {
  size_t i = addr.find_first_of(":");
  if (i == addr.npos) {
    ERRORLOG("invalid ipv4 addr :%s ", addr.c_str());
    return;
  }
  m_ip = addr.substr(0, i);
  m_port = std::atoi(addr.substr(i + 1, addr.size() - i - 1).c_str());

  memset(&m_addr, 0, sizeof(m_addr));
  m_addr.sin_family = AF_INET;
  m_addr.sin_addr.s_addr = inet_addr(m_ip.c_str());
  m_addr.sin_port = htons(m_port);
}
IpNetAddr::~IpNetAddr() {}

IpNetAddr::IpNetAddr(sockaddr_in addr) : m_addr(addr) {
  m_ip = std::string(inet_ntoa(m_addr.sin_addr));
  m_port = ntohs(m_addr.sin_port);
}
sockaddr *IpNetAddr::getSockAddr() {
  return reinterpret_cast<sockaddr *>(&m_addr);
}

socklen_t IpNetAddr::getSockLen() { return sizeof(m_addr); }

int IpNetAddr:: getFamily() { return AF_INET; }

std::string IpNetAddr::toString() {
  std::string re;
  re = re + ":" + std::to_string(m_port);

  return re;
}

bool IpNetAddr::checkValid() {
  if (m_ip.empty()) {
    return false;
  }

  if (m_port < 0 || m_port > 65536) {
    return false;
  }

  if(inet_addr(m_ip.c_str())==INADDR_NONE){
      return false;
  }
  return true;
}

} // namespace hsrpc