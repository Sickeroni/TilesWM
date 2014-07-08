TOP_DIR := .

include $(TOP_DIR)/config.mk

COMMON_DEPS :=
COMMON_DEPS += $(BUILD_DIR)/libfrontend.a
COMMON_DEPS += $(BUILD_DIR)/libfrontend_modes.a
COMMON_DEPS += $(BUILD_DIR)/libfrontend_container.a
COMMON_DEPS += $(BUILD_DIR)/libfrontend_container_container.a
COMMON_DEPS += $(BUILD_DIR)/libcommon.a

COMMON_LIBS :=
COMMON_LIBS += -lfrontend_modes
COMMON_LIBS += -lfrontend_container_container
COMMON_LIBS += -lfrontend_container
COMMON_LIBS += -lfrontend
COMMON_LIBS += -lcommon

# COMMON_DEPS := $(BUILD_DIR)/libcommon.a
# COMMON_LIBS := -lcommon

BACKEND_X11_DEPS := $(BUILD_DIR)/libbackend_x11.a
BACKEND_X11_LIBS := -lX11 -lXcursor -lbackend_x11

BACKEND_X11_NATIVE_DEPS := $(BUILD_DIR)/libbackend_x11_native.a
BACKEND_X11_NATIVE_LIBS := -lbackend_x11_native

BACKEND_X11_CAIRO_DEPS := $(BUILD_DIR)/libbackend_x11_cairo.a
BACKEND_X11_CAIRO_LIBS := -lbackend_x11_cairo -lcairomm-1.0

DEFAULT_TARGETS :=
DEFAULT_TARGETS += $(BUILD_DIR)/ttmwm
# DEFAULT_TARGETS += $(BUILD_DIR)/ttmwm-cairo


CXXFLAGS += -I$(TOP_DIR)/frontend
CXXFLAGS += -I$(TOP_DIR)/frontend_modes

.PHONY : all
all: $(DEFAULT_TARGETS)

.PHONY : tests
tests:
	@ (cd $(TOP_DIR)/tests && make)

$(BUILD_DIR)/lib%.a: $(TOP_DIR)/%/*.cpp $(TOP_DIR)/%/*.h
	@ (cd $(TOP_DIR)/$* &&  make)

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
	@ $(CXX) $(CXXFLAGS) -MD -I$(TOP_DIR)/common -c $*.cpp -o $@

.PHONY : doc
doc:
	BUILD_DIR=$(BUILD_DIR) doxygen Doxyfile_all
	BUILD_DIR=$(BUILD_DIR) doxygen Doxyfile_common

.PHONY : clean
clean: 
	rm -rf $(BUILD_DIR)/*
