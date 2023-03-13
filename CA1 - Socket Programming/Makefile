# target macros
TARGET_SERVER := ./src/server/server.out
TARGET_CLIENT := ./src/client/client.out
SERVER := ./src/server/main.cpp
CLIENT := ./src/client/main.cpp

# compile macros
DIRS := src # FILL: only the dirs which contain the src files, in this case, `src` should be added
SERVER_OBJS := ./src/server/main.o
CLIENT_OBJS := ./src/client/main.o

# intermedia compile macros
# NOTE: ALL_OBJS are intentionally left blank, no need to fill it
SERVER_ALL_OBJS := $(SERVER_OBJS)
CLIENT_ALL_OBJS := $(CLIENT_OBJS)
CLEAN_FILES := $(TARGET_SERVER) $(TARGET_CLIENT) $(SERVER_OBJS) $(CLIENT_OBJS)
DIST_CLEAN_FILES := $(SERVER_OBJS) $(CLIENT_OBJS)

# recursive wildcard
rwildcard=$(foreach d,$(wildcard $(addsuffix *,$(1))),$(call rwildcard,$(d)/,$(2))$(filter $(subst *,%,$(2)),$(d)))

# default target
default: show-info all

# non-phony targets
$(TARGET_SERVER): build-subdirs $(SERVER_ALL_OBJS) find-all-objs
	@echo -e "\t" CXX $(CFLAGS) $(SERVER_ALL_OBJS) -o $@
	@$(CXX) $(CFLAGS) $(SERVER_ALL_OBJS) -o $@

$(TARGET_CLIENT): build-subdirs $(CLIENT_ALL_OBJS) find-all-objs
	@echo -e "\t" CXX $(CFLAGS) $(CLIENT_ALL_OBJS) -o $@
	@$(CXX) $(CFLAGS) $(CLIENT_ALL_OBJS) -o $@

# phony targets
.PHONY: all
all: $(TARGET_SERVER) $(TARGET_CLIENT)
	@echo Target $(TARGET_SERVER) and $(TARGET_CLIENT) build finished.

.PHONY: clean
clean: clean-subdirs
	@echo CLEAN $(CLEAN_FILES)
	@rm -f $(CLEAN_FILES)

.PHONY: distclean
distclean: clean-subdirs
	@echo CLEAN $(DIST_CLEAN_FILES)
	@rm -f $(DIST_CLEAN_FILES)

# phony funcs
.PHONY: find-all-objs
find-all-objs:
	$(eval ALL_OBJS += $(call rwildcard,$(DIRS),*.o))

.PHONY: show-info
show-info:
	@echo Building Project

# need to be placed at the end of the file
mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
export PROJECT_PATH := $(patsubst %/,%,$(dir $(mkfile_path)))
export MAKE_INCLUDE=$(PROJECT_PATH)/config/make.global
export SUB_MAKE_INCLUDE=$(PROJECT_PATH)/config/submake.global
include $(MAKE_INCLUDE)
