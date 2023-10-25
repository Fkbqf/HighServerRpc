#include <assert.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
#include <memory>
#include <unistd.h>
#include <google/protobuf/service.h>
#include "hsrpc/common/log.h"
#include "hsrpc/common/config.h"
#include "hsrpc/common/log.h"
#include "hsrpc/net/tcp/tcp.clinet.h"
#include "hsrpc/net/tcp/net_addr.h"
#include "hsrpc/net/coder/string_coder.h"
#include "hsrpc/net/coder/abstact_protocol.h"
#include "hsrpc/net/coder/tinypb_coder.h"
#include "hsrpc/net/coder/tinypb_protocol.h"
#include "hsrpc/net/tcp/net_addr.h"
#include "hsrpc/net/tcp/tcp_server.h"
#include "hsrpc/net/rpc/rpc_dispatcher.h"
#include "hsrpc/net/rpc/rpc_controller.h"
#include "hsrpc/net/rpc/rpc_channel.h"
#include "hsrpc/net/rpc/rpc_closuer.h"

#include "order.pb.h"



void test_tcp_client() {

  hsrpc::IpNetAddr::s_ptr addr = std::make_shared<hsrpc::IpNetAddr>("127.0.0.1", 12345);
  hsrpc::TcpClient client(addr);
  client.connect([addr, &client]() {
    DEBUGLOG("conenct to [%s] success", addr->toString().c_str());
    std::shared_ptr<hsrpc::TinyPBProtocol> message = std::make_shared<hsrpc::TinyPBProtocol>();
    message->m_msg_id = "99998888";
    message->m_pb_data = "test pb data";

    makeOrderRequest request;
    request.set_price(100);
    request.set_goods("apple");
    
    if (!request.SerializeToString(&(message->m_pb_data))) {
      ERRORLOG("serilize error");
      return;
    }

    message->m_method_name = "Order.makeOrder";

    client.writeMessage(message, [request](hsrpc::AbstractProtocol::s_ptr msg_ptr) {
      DEBUGLOG("send message success, request[%s]", request.ShortDebugString().c_str());
    });


    client.readMessage("99998888", [](hsrpc::AbstractProtocol::s_ptr msg_ptr) {
      std::shared_ptr<hsrpc::TinyPBProtocol> message = std::dynamic_pointer_cast<hsrpc::TinyPBProtocol>(msg_ptr);
      DEBUGLOG("msg_id[%s], get response %s", message->m_msg_id.c_str(), message->m_pb_data.c_str());
      makeOrderResponse response;

      if(!response.ParseFromString(message->m_pb_data)) {
        ERRORLOG("deserialize error");
        return;
      }
      DEBUGLOG("get response success, response[%s]", response.ShortDebugString().c_str());
    });
  });
}

void test_rpc_channel() {

  NEWRPCCHANNEL("127.0.0.1:12345", channel);

  // std::shared_ptr<makeOrderRequest> request = std::make_shared<makeOrderRequest>();

  NEWMESSAGE(makeOrderRequest, request);
  NEWMESSAGE(makeOrderResponse, response);

  request->set_price(100);
  request->set_goods("apple");

  NEWRPCCONTROLLER(controller);
  controller->SetMsgId("99998888");
  controller->SetTimeout(10000);

  std::shared_ptr<hsrpc::RpcClosure> closure = std::make_shared<hsrpc::RpcClosure>([request, response, channel, controller]() mutable {
    if (controller->GetErrorCode() == 0) {
      INFOLOG("call rpc success, request[%s], response[%s]", request->ShortDebugString().c_str(), response->ShortDebugString().c_str());
      // 执行业务逻辑
      if (response->order_id() == "xxx") {
        // xx
      }
    } else {
      ERRORLOG("call rpc failed, request[%s], error code[%d], error info[%s]", 
        request->ShortDebugString().c_str(), 
        controller->GetErrorCode(), 
        controller->GetErrorInfo().c_str());
    }
  
    INFOLOG("now exit eventloop");
    // channel->getTcpClient()->stop();
    channel.reset();
  });

  CALLRPRC("127.0.0.1:12345", Order_Stub, makeOrder, controller, request, response, closure);

  // xxx
  // 协程
}

int main() {

  hsrpc::Config::SetGlobalConfig(NULL);

  hsrpc::Logger::InitGlobalLogger(0);

  // test_tcp_client();
  test_rpc_channel();

  INFOLOG("test_rpc_channel end");

  return 0;
}