#zlib库可选择性使用
#USE_ARM := N
#USE_SSL := N
#USE_ZLIB := N
EXECUTABLE := httpclient
CXXFLAGS = -pthread
INCLUDE_PATH := \
	-I./ 


#使用ARM交叉编译
ifeq ($(USE_ARM), Y)
OPENSSL_DIR := /opt/openssl-arm-ca9
CXX:=arm-ca9-linux-gnueabihf-g++
else
#使用GCC编译
OPENSSL_DIR := /opt/openssl-1.1.1l
CXX:=g++
endif

#使用OPENSSL
ifeq ($(USE_SSL), Y)
CXXFLAGS += -DCPPHTTPLIB_OPENSSL_SUPPORT 
LIBS += -L$(OPENSSL_DIR)/lib -lssl -lcrypto
INCLUDE_PATH += -I$(OPENSSL_DIR)/include
endif

#使用ZLIB
ifeq ($(USE_ZLIB), Y)
CXXFLAGS += -DCPPHTTPLIB_ZLIB_SUPPORT -lz
endif

SRCDIR := ./

SRCS := $(wildcard $(addsuffix *.cpp, $(SRCDIR)))
OBJS := $(patsubst %.cpp,%.o,$(SRCS))

CXXFLAGS += $(INCLUDE_PATH)

.PHONY: echo all clean

all: $(EXECUTABLE)

echo:
	ehco $(OBJS)

$(EXECUTABLE) : $(OBJS)
	$(CXX) -o $(EXECUTABLE) $(OBJS) $(CXXFLAGS) $(LIBS)
	

    
clean:
	-rm *.o $(EXECUTABLE)
