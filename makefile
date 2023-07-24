# 定义二进制文件的路径为 "bin"
PATH_BIN = bin

# 定义库文件的路径为 "lib"
PATH_LIB = lib

# 定义对象文件的路径为 "obj"
PATH_OBJ = obj

# 定义 "rocket" 目录的路径
PATH_ROCKET = rocket

# 定义 "common" 目录的路径，它在 "rocket" 目录下
PATH_COMM = $(PATH_ROCKET)/common

# 定义 "net" 目录的路径，它在 "rocket" 目录下
PATH_NET = $(PATH_ROCKET)/net

# 定义 "testcases" 测试用例的目录路径
PATH_TESTCASES = testcases

# 定义库文件的安装路径为 "/usr/lib"
# will install lib to /usr/lib/libsocket.a
PATH_INSTALL_LIB_ROOT = /usr/lib

# 定义头文件的安装路径为 "/usr/include"
# will install all header file to /usr/include/socket
PATH_INSTALL_INC_ROOT = /usr/include

# 定义 "common" 头文件的安装路径，它将安装到 "/usr/include/rocket/common"
PATH_INSTALL_INC_COMM = $(PATH_INSTALL_INC_ROOT)/$(PATH_COMM)

# 定义 "net" 头文件的安装路径，它将安装到 "/usr/include/rocket/net"
PATH_INSTALL_INC_NET = $(PATH_INSTALL_INC_ROOT)/$(PATH_NET)


# PATH_PROTOBUF = /usr/include/google
# PATH_TINYXML = /usr/include/tinyxml

CXX := g++

CXXFLAGS += -g -O0 -std=c++11 -Wall -Wno-deprecated -Wno-unused-but-set-variable

#添加项目的头文件路径到编译器的搜索路径中
CXXFLAGS += -I./ -I$(PATH_ROCKET)	-I$(PATH_COMM) -I$(PATH_NET)

LIBS += /usr/lib/libprotobuf.so	/usr/lib/libtinyxml.a

#将 "common" 目录下的所有 .cc 文件转换为 .o 对象文件。将 "net" 目录下的所有 .cc 文件转换为 .o 对象文件。
COMM_OBJ := $(patsubst $(PATH_COMM)/%.cc, $(PATH_OBJ)/%.o, $(wildcard $(PATH_COMM)/*.cc))
NET_OBJ := $(patsubst $(PATH_NET)/%.cc, $(PATH_OBJ)/%.o, $(wildcard $(PATH_NET)/*.cc))

ALL_TESTS : $(PATH_BIN)/test_log $(PATH_BIN)/test_eventloop

TEST_CASE_OUT := $(PATH_BIN)/test_log $(PATH_BIN)/test_eventloop

LIB_OUT := $(PATH_LIB)/librocket.a

$(PATH_BIN)/test_log: $(LIB_OUT)
	$(CXX) $(CXXFLAGS) $(PATH_TESTCASES)/test_log.cc -o $@ $(LIB_OUT) $(LIBS) -ldl -pthread

$(PATH_BIN)/test_eventloop: $(LIB_OUT)
	$(CXX) $(CXXFLAGS) $(PATH_TESTCASES)/test_eventloop.cc -o $@ $(LIB_OUT) $(LIBS) -ldl -pthread


$(LIB_OUT): $(COMM_OBJ) $(NET_OBJ)
	cd $(PATH_OBJ) && ar rcv librocket.a *.o && cp librocket.a ../lib/

$(PATH_OBJ)/%.o : $(PATH_COMM)/%.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@


$(PATH_OBJ)/%.o : $(PATH_NET)/%.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@


# print something test
# like this: make PRINT-PATH_BIN, and then will print variable PATH_BIN
PRINT-% : ; @echo $* = $($*)


# to clean 
clean :
	rm -f $(COMM_OBJ) $(NET_OBJ) $(TESTCASES) $(TEST_CASE_OUT) $(PATH_LIB)/librocket.a $(PATH_OBJ)/librocket.a

# install
install:
	mkdir -p $(PATH_INSTALL_INC_COMM) $(PATH_INSTALL_INC_NET) \
		&& cp $(PATH_COMM)/*.h $(PATH_INSTALL_INC_COMM) \
		&& cp $(PATH_NET)/*.h $(PATH_INSTALL_INC_NET) \
		&& cp $(LIB_OUT) $(PATH_INSTALL_LIB_ROOT)/


# uninstall
uninstall:
	rm -rf $(PATH_INSTALL_INC_ROOT)/ROCKET && rm -f $(PATH_INSTALL_LIB_ROOT)/librocket.a