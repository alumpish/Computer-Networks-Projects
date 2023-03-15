mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
PROJECT_PATH := $(patsubst %/,%,$(dir $(mkfile_path)))

# compile macro
CC := gcc
CXX:= g++

# compile flags
CFLAGS :=
CXXFLAGS := -I $(PROJECT_PATH)/src/common -I $(PROJECT_PATH)/src/lib/nlohmann
OBJCFLAGS := $(CFLAGS) -c
OBJCXXFLAGS := $(CXXFLAGS) -c

# target macros
TARGET_SERVER := ./src/server/server.out
TARGET_CLIENT := ./src/client/client.out

DIRS := src

COMMON_OBJS := $(patsubst %.cpp, %.o, $(wildcard ./src/common/*.cpp))
SERVER_OBJS := $(patsubst %.cpp, %.o, $(wildcard ./src/server/*.cpp))
CLIENT_OBJS := $(patsubst %.cpp, %.o, $(wildcard ./src/client/*.cpp))
CLEAN_FILES := $(TARGET_SERVER) $(TARGET_CLIENT) $(SERVER_OBJS) $(CLIENT_OBJS) $(COMMON_OBJS)
DIST_CLEAN_FILES := $(SERVER_OBJS) $(CLIENT_OBJS) $(COMMON_OBJS)

# default target
default: all

# non-phony targets
$(TARGET_SERVER): $(SERVER_OBJS) $(COMMON_OBJS)
	$(CXX) $(SERVER_OBJS) $(COMMON_OBJS) -o $@

$(TARGET_CLIENT): $(CLIENT_OBJS) $(COMMON_OBJS)
	$(CXX) $(CLIENT_OBJS) $(COMMON_OBJS) -o $@

%.o: %.cpp
	$(CXX) $(OBJCXXFLAGS) $< -o $@

# phony targets
.PHONY: all
all: $(TARGET_SERVER) $(TARGET_CLIENT)
	@echo Target $(TARGET_SERVER) and $(TARGET_CLIENT) build finished.

.PHONY: clean
clean:
	rm -f $(CLEAN_FILES)

.PHONY: distclean
distclean:
	rm -f $(DIST_CLEAN_FILES)
