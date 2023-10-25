#ifndef hsrpc_CODER_ABSTRACT_PROTOCOL_H
#define hsrpc_CODER_ABSTRACT_PROTOCOL_H

#include <memory>
#include <string>

namespace hsrpc {

class AbstractProtocol  : public std::enable_shared_from_this<AbstractProtocol>{
public:
  typedef std::shared_ptr<AbstractProtocol> s_ptr;

  std::string getMsgId() { return m_msg_id; }
  void setMsgId(const std::string &req_id) { m_msg_id = req_id; }

  virtual ~AbstractProtocol(){}

public:
  std::string m_msg_id;
};

} // namespace hsrpc

#endif
