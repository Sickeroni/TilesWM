CXXFLAGS += -Werror
CXXFLAGS += -Wfatal-errors
CXXFLAGS += -Wall
CXXFLAGS += -Wuninitialized
CXXFLAGS += -Woverloaded-virtual
CXXFLAGS += -O0
CXXFLAGS += -g
CXXFLAGS += -std=c++11
#CXXFLAGS += -fno-strict-aliasing
#CXXFLAGS += -fstrict-aliasing
#CXXFLAGS += -fpermissive

CXXFLAGS += -I/usr/include/cairomm-1.0
CXXFLAGS += -I/usr/include/sigc++-2.0
CXXFLAGS += -I/usr/include/cairo
CXXFLAGS += -I/usr/include/freetype2
# workaround debian bug
CXXFLAGS += -I/usr/lib/x86_64-linux-gnu/sigc++-2.0/include

BUILD_DIR := $(TOP_DIR)/build
