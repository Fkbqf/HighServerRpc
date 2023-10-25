#ifndef hsrpc_COMMON_MSGID_UTIL_H
#define hsrpc_COMMON_MSGID_UTIL_H

#include <string>


namespace hsrpc {

class MsgIDUtil {

 public:
  static std::string GenMsgID();

};

}


#endif