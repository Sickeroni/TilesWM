include config.mk

BUILD_DIR=build

COMMON_DEPS := $(BUILD_DIR)/libfrontend.a $(BUILD_DIR)/libcommon.a
COMMON_LIBS := -lfrontend -lcommon

BACKEND_X11_DEPS := $(BUILD_DIR)/libbackend_x11.a
BACKEND_X11_LIBS := -lX11 -lXcursor -lbackend_x11

BACKEND_X11_NATIVE_DEPS := $(BUILD_DIR)/libbackend_x11_native.a
BACKEND_X11_NATIVE_LIBS := -lbackend_x11_native

BACKEND_X11_CAIRO_DEPS := $(BUILD_DIR)/libbackend_x11_cairo.a
BACKEND_X11_CAIRO_LIBS := -lbackend_x11_cairo -lcairomm-1.0

DEFAULT_TARGETS :=
DEFAULT_TARGETS += $(BUILD_DIR)/ttmwm
# DEFAULT_TARGETS += $(BUILD_DIR)/ttmwm-cairo


.PHONY : all
all: $(DEFAULT_TARGETS)

$(BUILD_DIR)/lib%.a: %/*.cpp %/*.h
	(mkdir -p $(BUILD_DIR)/$* && cd $* &&  make BUILD_DIR=../$(BUILD_DIR)/$* OUTPUT_DIR=../$(BUILD_DIR) ../$(BUILD_DIR)/lib$*.a)

$(BUILD_DIR)/ttmwm: $(COMMON_DEPS) $(BACKEND_X11_DEPS) $(BACKEND_X11_NATIVE_DEPS) $(BUILD_DIR)/ttmwm.o
	@ echo linking $@
	@ $(CXX) $(CXXFLAGS) -L$(BUILD_DIR) $(BUILD_DIR)/ttmwm.o $(BACKEND_X11_LIBS) $(BACKEND_X11_NATIVE_LIBS) $(COMMON_LIBS) -o $@

$(BUILD_DIR)/ttmwm-cairo: $(COMMON_DEPS) $(BACKEND_X11_DEPS) $(BACKEND_X11_CAIRO_DEPS) $(BUILD_DIR)/ttmwm.o
	@ echo linking $@
	@ $(CXX) $(CXXFLAGS) -L$(BUILD_DIR) $(BUILD_DIR)/ttmwm.o $(BACKEND_X11_LIBS) $(BACKEND_X11_CAIRO_LIBS) $(COMMON_LIBS) -o $@


# pull in dependency info for *existing* .o files
-include $(BUILD_DIR)/ttmwm.d

# compile and generate dependency info
$(BUILD_DIR)/%.o: %.cpp
	@ echo $*.cpp
	@ $(CXX) $(CXXFLAGS) -MD -Icommon -c $*.cpp -o $@

.PHONY : doc
doc:
	BUILD_DIR=$(BUILD_DIR) doxygen Doxyfile_all
	BUILD_DIR=$(BUILD_DIR) doxygen Doxyfile_common

.PHONY : clean
clean: 
	rm -rf $(BUILD_DIR)/*
