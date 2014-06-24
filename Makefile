include make_conf

CXXFLAGS += -I/usr/include/cairomm-1.0
CXXFLAGS += -I/usr/include/sigc++-2.0
CXXFLAGS += -I/usr/include/cairo
CXXFLAGS += -I/usr/include/freetype2
# workaround debian bug
CXXFLAGS += -I/usr/lib/x86_64-linux-gnu/sigc++-2.0/include

LIBCOMMON_SRCS :=
LIBCOMMON_SRCS += mouse_handler
LIBCOMMON_SRCS += action_set
LIBCOMMON_SRCS += shortcut_set
LIBCOMMON_SRCS += config
LIBCOMMON_SRCS += string_util
LIBCOMMON_SRCS += container_container_layout
LIBCOMMON_SRCS += client_container_layout
LIBCOMMON_SRCS += theme
LIBCOMMON_SRCS += workspace
LIBCOMMON_SRCS += container
LIBCOMMON_SRCS += container_container
LIBCOMMON_SRCS += client_container
LIBCOMMON_SRCS += client
LIBCOMMON_SRCS += monitor
LIBCOMMON_SRCS += application
LIBCOMMON_SRCS += main_actions
LIBCOMMON_SRCS += container_util
LIBCOMMON_SRCS += mode_default
LIBCOMMON_SRCS += mode_3panel
LIBCOMMON_SRCS += cairo_canvas
LIBCOMMON_SRCS += cairo_icon
LIBCOMMON_SRCS += window_manager_default

TTMWM_X11_SRCS :=
TTMWM_X11_SRCS += default_config_values
TTMWM_X11_SRCS += x11_icon
TTMWM_X11_SRCS += x11_canvas
TTMWM_X11_SRCS += x11_container_container
TTMWM_X11_SRCS += x11_client_container
TTMWM_X11_SRCS += x11_widget
TTMWM_X11_SRCS += x11_server_widget
TTMWM_X11_SRCS += x11_client_widget
TTMWM_X11_SRCS += x11_client
TTMWM_X11_SRCS += x11_shortcut_set
TTMWM_X11_SRCS += x11_application
TTMWM_X11_SRCS += x11_global
TTMWM_X11_SRCS += main


LIBCOMMON_OBJS := $(patsubst %,$(BUILD_DIR)/common/%.o,$(LIBCOMMON_SRCS))
TTMWM_X11_OBJS := $(patsubst %,$(BUILD_DIR)/x11/%.o,$(TTMWM_X11_SRCS))


DEFAULT_TARGETS :=
DEFAULT_TARGETS += $(BUILD_DIR)/ttmwm
DEFAULT_TARGETS += $(BUILD_DIR)/ttmwm-cairo


.PHONY : all
all: $(DEFAULT_TARGETS)

$(BUILD_DIR)/common:
	@ mkdir -p $@

$(BUILD_DIR)/x11:
	@ mkdir -p $@


$(BUILD_DIR)/libcommon.a: $(BUILD_DIR)/common $(LIBCOMMON_OBJS)
	@ echo creating static library $@
	@ ar rcs $@ $(LIBCOMMON_OBJS)


$(BUILD_DIR)/ttmwm: $(BUILD_DIR)/libcommon.a $(BUILD_DIR)/x11 $(TTMWM_X11_OBJS) $(BUILD_DIR)/x11/x11_graphics_system_x11.o
	@ echo linking $@
	@ $(CXX) $(CXXFLAGS) -L$(BUILD_DIR) $(TTMWM_X11_OBJS) $(BUILD_DIR)/x11/x11_graphics_system_x11.o -lX11 -lcommon -o $@

$(BUILD_DIR)/ttmwm-cairo: $(BUILD_DIR)/libcommon.a $(BUILD_DIR)/x11 $(TTMWM_X11_OBJS) $(BUILD_DIR)/x11/x11_graphics_system_cairo.o
	@ echo linking $@
	@ $(CXX) $(CXXFLAGS) -L$(BUILD_DIR) $(TTMWM_X11_OBJS) $(BUILD_DIR)/x11/x11_graphics_system_cairo.o -lX11 -lcairomm-1.0 -lcommon -o $@

# pull in dependency info for *existing* .o files
# from gnu make manual:
# If you want make to simply ignore a makefile which does not exist or cannot be remade,
# with no error message, use the -include directive instead of include
-include $(patsubst %,$(BUILD_DIR)/common/%.d,$(LIBCOMMON_SRCS))
-include $(patsubst %,$(BUILD_DIR)/x11/%.d,$(TTMWM_X11_SRCS))

# compile and generate dependency info
$(BUILD_DIR)/%.o: %.cpp
	@ echo $*.cpp
	@ $(CXX) $(CXXFLAGS) -MD -Icommon -c $*.cpp -o $@


.PHONY : clean
clean: 
	rm -rf $(BUILD_DIR)/*
