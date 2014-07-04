TOP_DIR = ..

include $(TOP_DIR)/config.mk

# COMMON_DEPS := $(BUILD_DIR)/libfrontend.a $(BUILD_DIR)/libcommon.a
# COMMON_LIBS := -lfrontend -lcommon
COMMON_DEPS := $(BUILD_DIR)/libcommon.a
COMMON_LIBS := -lcommon

BACKEND_X11_DEPS := $(BUILD_DIR)/libbackend_x11.a
BACKEND_X11_LIBS := -lX11 -lXcursor -lbackend_x11

BACKEND_X11_NATIVE_DEPS := $(BUILD_DIR)/libbackend_x11_native.a
BACKEND_X11_NATIVE_LIBS := -lbackend_x11_native

BACKEND_X11_CAIRO_DEPS := $(BUILD_DIR)/libbackend_x11_cairo.a
BACKEND_X11_CAIRO_LIBS := -lbackend_x11_cairo -lcairomm-1.0


TESTS_BUILD_DIR := $(BUILD_DIR)/tests

DEFAULT_TARGETS :=
DEFAULT_TARGETS += $(TESTS_BUILD_DIR)/x11_backend_test

.PHONY : all
all: $(DEFAULT_TARGETS)

$(BUILD_DIR)/lib%.a: $(TOP_DIR)/%/*.cpp $(TOP_DIR)/%/*.h
	@ (cd $(TOP_DIR)/$* &&  make)

$(TESTS_BUILD_DIR):
	@ mkdir -p $@

$(TESTS_BUILD_DIR)/x11_backend_test: $(COMMON_DEPS) $(BACKEND_X11_DEPS) $(BACKEND_X11_NATIVE_DEPS) $(TESTS_BUILD_DIR) $(TESTS_BUILD_DIR)/x11_backend_test.o
	@ echo linking $@
	@ $(CXX) $(CXXFLAGS) -L$(BUILD_DIR) $(TESTS_BUILD_DIR)/x11_backend_test.o $(BACKEND_X11_LIBS) $(BACKEND_X11_NATIVE_LIBS) $(COMMON_LIBS) -o $@


# pull in dependency info for *existing* .o files
-include $(TESTS_BUILD_DIR)/x11_backend_test.d

# compile and generate dependency info
$(TESTS_BUILD_DIR)/%.o: %.cpp
	@ echo $*.cpp
	@ $(CXX) $(CXXFLAGS) -MD -I$(TOP_DIR)/common -c $*.cpp -o $@


.PHONY : clean
clean:
	rm -rf $(TESTS_BUILD_DIR)/*
