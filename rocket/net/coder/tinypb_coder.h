#ifndef ROCKET_CODER_TINYPB_CODER_H
#define ROCKET_CODER_TINYPB_CODER_H
#include "rocket/net/coder/abstact_coder.h"
#include "rocket/net/coder/tinypb_protocol.h"
namespace rocket {

class TinyPBCoder : public AbstractCoder {

public:
  virtual void encode(std::vector<AbstractProtocol::s_ptr> &messages,
                      TcpBuffer::s_ptr out_buffer) ;

  // 将 buffer 里面的字节流转换为 message 对象
  virtual void decode(std::vector<AbstractProtocol::s_ptr> &out_messages,
                      TcpBuffer::s_ptr buffer) ;

private:
  const char *encodeTinyPB(std::shared_ptr<TinyPBProtocol> message, int &len);
};
} // namespace rocket

#endif