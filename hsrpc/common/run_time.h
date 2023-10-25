#ifndef hsrpc_COMMON_RUN_TIME_H
#define hsrpc_COMMON_RUN_TIME_H


#include <string>

namespace hsrpc {

class RunTime {
 public:

 public:
  static RunTime* GetRunTime();

 public:
  std::string m_msgid;
  std::string m_method_name;

};

}


#endif