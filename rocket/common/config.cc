#include <tinyxml/tinyxml.h>
#include "rocket/common/config.h"



// 用于从指定的 XML 节点中读取一个指定名称的子节点，并进行错误处理。
// 具体来说，它会在指定节点的父节点中查找名为 name 的子节点，并将结果保存到名为 name##_node 的指针变量中。
// 如果没有找到该节点，则输出一条错误消息并退出程序。
#define READ_XML_NODE(name, parent) \
TiXmlElement* name##_node = parent->FirstChildElement(#name); \
if (!name##_node) { \
  printf("Start rocket server error, failed to read node [%s]\n", #name); \
  exit(0); \
} \

// 用于从指定的 XML 节点中读取一个指定名称的字符串子节点，并进行错误处理。
// 具体来说，它会在指定节点的父节点中查找名为 name 的子节点，并将结果保存到名为 name##_str 的字符串变量中。
// 如果没有找到该节点或者该节点的文本内容为空，则输出一条错误消息并退出程序。
#define READ_STR_FROM_XML_NODE(name, parent) \
  TiXmlElement* name##_node = parent->FirstChildElement(#name); \
  if (!name##_node|| !name##_node->GetText()) { \
    printf("Start rocket server error, failed to read config file %s\n", #name); \
    exit(0); \
  } \
  std::string name##_str = std::string(name##_node->GetText()); \


namespace rocket {


static Config* g_config = NULL;


Config* Config::GetGlobalConfig() {
  return g_config;
}

void Config::SetGlobalConfig(const char* xmlfile) {
  if (g_config == NULL) {
    g_config = new Config(xmlfile);
  }
}
  
Config::Config(const char* xmlfile) {
  TiXmlDocument* xml_document = new TiXmlDocument();

  bool rt = xml_document->LoadFile(xmlfile);
  if (!rt) {
    printf("Start rocket server error, failed to read config file %s, error info[%s] \n", xmlfile, xml_document->ErrorDesc());
    exit(0);
  }

  READ_XML_NODE(root, xml_document);
  READ_XML_NODE(log, root_node);


  READ_STR_FROM_XML_NODE(log_level, log_node);

  m_log_level = log_level_str;

}


}