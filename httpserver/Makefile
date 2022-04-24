
OPENSSL_DIR := /opt/openssl-1.1.1l
EXECUTABLE := httpserver
CXXFLAGS = -O2 -std=c++11  -Wall -pthread -DCPPHTTPLIB_OPENSSL_SUPPORT
INCLUDE_PATH := \
	-I./ \
	-I$(OPENSSL_DIR)/include

LIBS += -L$(OPENSSL_DIR)/lib -lssl -lcrypto

CC:=g++
SRCDIR := ./

SRCS := $(wildcard $(addsuffix *.cpp, $(SRCDIR)))
OBJS := $(patsubst %.cpp,%.o,$(SRCS))

CXXFLAGS += $(INCLUDE_PATH)

.PHONY: echo all clean pem

all: $(EXECUTABLE)

echo:
	ehco $(OBJS)

$(EXECUTABLE) : $(OBJS)
	$(CXX) -o $(EXECUTABLE) $(OBJS) $(CXXFLAGS) $(LIBS)
	
pem:
	openssl genrsa 2048 > key.pem
	openssl req -new -key key.pem | openssl x509 -days 3650 -req -signkey key.pem > cert.pem
    
clean:
	rm *.o *.pem $(EXECUTABLE)
