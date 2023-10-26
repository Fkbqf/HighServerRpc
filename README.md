

## 主要特性

### 主从Reactor架构与epoll技术

- 利用**epoll的IO多路复用技术**，保证高并发下的响应速度。
- 结合主从Reactor架构，提高系统的稳定性和伸缩性。

### "one loop per thread"与thread pool策略

- 受muduo库的启发，采纳了这一设计策略。
- 确保了高并发情境下的性能表现，大幅提升了系统的并发性。

### 异步RPC调用与Reactor处理

- 支持注册回调的**异步RPC调用**。
- 通过Reactor异步处理操作，减少阻塞，提高响应速度。

### TinyPB协议

- 引入了基于Protobuf的自定义**TinyPB协议**。
- 高效且稳定，保证了数据传输的准确性和安全性。

### 高效日志管理

- 设计了细致且强大的日志模块。
- 支持按日期和大小的日志滚动，确保日志信息的完整性。
- 通过多缓冲区技术实现异步线程日志输出，大幅提高了日志输出的吞吐量。
- 提供了框架与业务两种日志模式，简化了调试过程，增强了日志的可读性。

### XML配置模块集成

- 框架集成了**XML配置模块**，自动化配置加载。
- 简化了系统的配置管理，降低了配置错误的风险。
