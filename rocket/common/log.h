#ifndef ROCKET_COMMON_LOG_H
#define ROCKET_COMMON_LOG_H

#include <string>
#include <queue>
#include <memory>

#include "rocket/common/config.h"
#include "rocket/common/mutex.h"

namespace rocket {


template<typename... Args>
std::string formatString(const char* str, Args&&... args) {

  int size = snprintf(nullptr, 0, str, args...);

  std::string result;
  if (size > 0) {
    result.resize(size);
    snprintf(&result[0], size + 1, str, args...);
  }

  return result;
}


#define DEBUGLOG(str, ...) \
  if (rocket::Logger::GetGlobalLogger()->getLogLevel() <= rocket::Debug) \
  { \
    rocket::Logger::GetGlobalLogger()->pushLog((new rocket::LogEvent(rocket::LogLevel::Debug))->toString() \
      + "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]\t" + rocket::formatString(str, ##__VA_ARGS__) + "\n");\
    rocket::Logger::GetGlobalLogger()->log();                                                                                \
  } \


#define INFOLOG(str, ...) \
  if (rocket::Logger::GetGlobalLogger()->getLogLevel() <= rocket::Info) \
  { \
  rocket::Logger::GetGlobalLogger()->pushLog((new rocket::LogEvent(rocket::LogLevel::Info))->toString() \
    + "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]\t" + rocket::formatString(str, ##__VA_ARGS__) + "\n");\
  rocket::Logger::GetGlobalLogger()->log();                                                                      \
  } \

#define ERRORLOG(str, ...) \
  if (rocket::Logger::GetGlobalLogger()->getLogLevel() <= rocket::Error) \
  { \
    rocket::Logger::GetGlobalLogger()->pushLog((new rocket::LogEvent(rocket::LogLevel::Error))->toString() \
      + "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]\t" + rocket::formatString(str, ##__VA_ARGS__) + "\n");\
    rocket::Logger::GetGlobalLogger()->log();                                                                                 \
  } \



enum LogLevel {
  Unknown = 0,
  Debug = 1,
  Info = 2,
  Error = 3
};



class Logger {
 public:
  // 定义智能指针类型，便于管理 Logger 对象的生命周期
  typedef std::shared_ptr<Logger> s_ptr;

  // 构造函数：初始化 Logger 对象的日志记录等级
  Logger(LogLevel level) : m_set_level(level) {}

  // 将一条日志消息加入到缓冲队列中
  void pushLog(const std::string& msg);

  // 输出所有缓冲的日志消息
  void log();

  // 获取当前 Logger 对象的日志记录等级
  LogLevel getLogLevel() const {
    return m_set_level;
  }

 public:
  // 获取全局 Logger 对象的指针
  static Logger* GetGlobalLogger();

  // 初始化全局 Logger 对象
  static void InitGlobalLogger();

 private:
  // 当前 Logger 对象的日志记录等级
  LogLevel m_set_level;

  // 缓冲队列，用于存储尚未输出的日志消息
  std::queue<std::string> m_buffer;

  // 互斥锁，用于保护缓冲队列的并发访问
  Mutex m_mutex;

};


std::string LogLevelToString(LogLevel level);

LogLevel StringToLogLevel(const std::string& log_level);
class LogEvent {
 public:

  // 构造函数：初始化 LogEvent 对象的日志记录等级
  LogEvent(LogLevel level) : m_level(level) {}

  // 获取文件名
  std::string getFileName() const {
    return m_file_name;  
  }

  // 获取日志记录等级
  LogLevel getLogLevel() const {
    return m_level;
  }

  // 将当前 LogEvent 对象转换成字符串格式，以便输出日志消息
  std::string toString();

 private:
  // 文件名
  std::string m_file_name;

  // 行号
  int32_t m_file_line;

  // 进程号
  int32_t m_pid;

  // 线程号
  int32_t m_thread_id;

  // 日志记录等级
  LogLevel m_level;

};


}

#endif