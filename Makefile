include make_conf

CXXFLAGS += -I/usr/include/cairomm-1.0
CXXFLAGS += -I/usr/include/sigc++-2.0
CXXFLAGS += -I/usr/include/cairo
CXXFLAGS += -I/usr/include/freetype2
# workaround debian bug
CXXFLAGS += -I/usr/lib/x86_64-linux-gnu/sigc++-2.0/include

COMMON_SRCS :=
# COMMON_SRCS += mouse_handler
COMMON_SRCS += common/shortcut_set
COMMON_SRCS += common/config
COMMON_SRCS += common/string_util
# COMMON_SRCS += container_container_layout
# COMMON_SRCS += client_container_layout
COMMON_SRCS += common/theme
# COMMON_SRCS += client_util
# COMMON_SRCS += application
# COMMON_SRCS += container_util
# COMMON_SRCS += mode_default
# COMMON_SRCS += mode_3pane
COMMON_SRCS += common/cairo_canvas
COMMON_SRCS += common/cairo_icon
# COMMON_SRCS += window_manager_default
# COMMON_SRCS += window_manager_3panel

FRONTEND_SRCS :=
FRONTEND_SRCS += main_actions
FRONTEND_SRCS += mode_simple
FRONTEND_SRCS += monitor
FRONTEND_SRCS += widget
FRONTEND_SRCS += child_widget
FRONTEND_SRCS += workspace
# FRONTEND_SRCS += container
FRONTEND_SRCS += client
# FRONTEND_SRCS += container_container
# FRONTEND_SRCS += client_container
FRONTEND_SRCS += action_set

BACKEND_X11_SRCS :=
BACKEND_X11_SRCS += x11/x11_widget_backend
BACKEND_X11_SRCS += x11/x11_widget
BACKEND_X11_SRCS += x11/x11_server_widget
BACKEND_X11_SRCS += x11/x11_client_widget
BACKEND_X11_SRCS += x11/x11_client
BACKEND_X11_SRCS += x11/x11_shortcut_set
BACKEND_X11_SRCS += x11/x11_application
BACKEND_X11_SRCS += x11/x11_global

BACKEND_X11_NATIVE_SRCS :=
BACKEND_X11_NATIVE_SRCS += x11/x11_canvas
BACKEND_X11_NATIVE_SRCS += x11/x11_icon
BACKEND_X11_NATIVE_SRCS += x11/x11_graphics_system_x11

objs_from_srcs = $(patsubst %,$(BUILD_DIR)/%.o,$(1))

COMMON_OBJS := $(call objs_from_srcs, $(COMMON_SRCS))
libfrontend_OBJS := $(call objs_from_srcs, $(FRONTEND_SRCS))
BACKEND_X11_OBJS := $(call objs_from_srcs, $(BACKEND_X11_SRCS))
BACKEND_X11_NATIVE_OBJS := $(call objs_from_srcs, $(BACKEND_X11_NATIVE_SRCS))

# BACKEND_X11 := $(BUILD_DIR)/libbackend_x11.a
# BACKEND_X11_CAIRO := $(BUILD_DIR)/libbackend_x11_cairo.a
# BACKEND_X11_NATIVE := $(BUILD_DIR)/libbackend_x11_native.a

DEFAULT_TARGETS :=
DEFAULT_TARGETS += $(BUILD_DIR)/ttmwm
# DEFAULT_TARGETS += $(BUILD_DIR)/ttmwm-cairo


.PHONY : all
all: $(DEFAULT_TARGETS)


$(BUILD_DIR)/common:
	@ mkdir -p $@

$(BUILD_DIR)/frontend:
	@ mkdir -p $@

$(BUILD_DIR)/x11:
	@ mkdir -p $@

$(BUILD_DIR)/libcommon.a: $(BUILD_DIR)/common $(COMMON_OBJS)
	@ echo creating static library $@
	@ ar rcs $@ $(COMMON_OBJS)
	
$(BUILD_DIR)/libbackend_x11.a: $(BUILD_DIR)/x11 $(BACKEND_X11_OBJS)
	@ echo creating static library $@
	@ ar rcs $@ $(BACKEND_X11_OBJS)

$(BUILD_DIR)/libbackend_x11_native.a: $(BUILD_DIR)/x11 $(BACKEND_X11_NATIVE_OBJS)
	@ echo creating static library $@
	@ ar rcs $@ $(BACKEND_X11_NATIVE_OBJS)

# $(BUILD_DIR)/libfrontend.a: $(BUILD_DIR)/frontend $(FRONTEND_OBJS)
# 	@ echo creating static library $@
# 	@ ar rcs $@ $(FRONTEND_OBJS)

# $(BUILD_DIR)/x11/x11_graphics_system_x11.o
	
$(BUILD_DIR)/ttmwm: $(BUILD_DIR)/libcommon.a $(BUILD_DIR)/libbackend_x11.a $(BUILD_DIR)/libbackend_x11_native.a $(BUILD_DIR)/ttmwm.o
	@ echo linking $@
	@ $(CXX) $(CXXFLAGS) -L$(BUILD_DIR) $(BUILD_DIR)/ttmwm.o -lX11 -lXcursor -lbackend_x11 -lbackend_x11_native -lcommon -o $@

# $(BUILD_DIR)/ttmwm-cairo: $(BUILD_DIR)/libcommon.a $(BUILD_DIR)/x11 $(TTMWM_X11_OBJS) $(BUILD_DIR)/x11/x11_graphics_system_cairo.o
# 	@ echo linking $@
# 	@ $(CXX) $(CXXFLAGS) -L$(BUILD_DIR) $(TTMWM_X11_OBJS) $(BUILD_DIR)/x11/x11_graphics_system_cairo.o -lX11 -lXcursor -lcairomm-1.0 -lcommon -o $@

# pull in dependency info for *existing* .o files
# from gnu make manual:
# If you want make to simply ignore a makefile which does not exist or cannot be remade,
# with no error message, use the -include directive instead of include
# -include $(patsubst %,$(BUILD_DIR)/common/%.d,$(COMMON_SRCS))
# -include $(patsubst %,$(BUILD_DIR)/x11/%.d,$(TTMWM_X11_SRCS))

# compile and generate dependency info
$(BUILD_DIR)/%.o: %.cpp
	@ echo $*.cpp
	@ $(CXX) $(CXXFLAGS) -Icommon -c $*.cpp -o $@


#@ $(CXX) $(CXXFLAGS) -MD -Icommon -c $*.cpp -o $@

.PHONY : doc
doc:
	BUILD_DIR=$(BUILD_DIR) doxygen Doxyfile
	BUILD_DIR=$(BUILD_DIR) doxygen Doxyfile_common

.PHONY : clean
clean: 
	rm -rf $(BUILD_DIR)/*
