#ifndef ROCKET_COMMON_CONFIG_H
#define ROCKET_COMMON_CONFIG_H

#include <map>
#include <string>

namespace rocket {

// Config 类
class Config {
 public:
  // 构造函数，参数为一个 XML 文件的路径
  Config(const char* xmlfile);

 public:
  // 静态方法，用于获取全局的 Config 实例
  static Config* GetGlobalConfig();

  // 静态方法，用于设置全局的 Config 实例，参数为一个 XML 文件的路径
  static void SetGlobalConfig(const char* xmlfile);

 public:
  // 成员变量，表示日志级别
  std::string m_log_level;
};

}


#endif