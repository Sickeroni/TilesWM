include make_conf


LIBCOMMON_SRCS :=
LIBCOMMON_SRCS += mouse_handler
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
LIBCOMMON_SRCS += actions
LIBCOMMON_SRCS += container_util
LIBCOMMON_SRCS += mode
LIBCOMMON_SRCS += mode_default
LIBCOMMON_SRCS += mode_3panel

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
TTMWM_X11_SRCS += x11_shortcut
TTMWM_X11_SRCS += x11_shortcut_set
TTMWM_X11_SRCS += x11_application
TTMWM_X11_SRCS += x11_global
TTMWM_X11_SRCS += main


LIBCOMMON_OBJS := $(patsubst %,$(BUILD_DIR)/common/%.o,$(LIBCOMMON_SRCS))
TTMWM_X11_OBJS := $(patsubst %,$(BUILD_DIR)/x11/%.o,$(TTMWM_X11_SRCS))


.PHONY : all
all: $(BUILD_DIR)/ttmwm

$(BUILD_DIR)/common:
	@ mkdir -p $@

$(BUILD_DIR)/x11:
	@ mkdir -p $@


$(BUILD_DIR)/libcommon.a: $(BUILD_DIR)/common $(LIBCOMMON_OBJS)
	@ echo creating static library $@
	@ ar rcs $@ $(LIBCOMMON_OBJS)


$(BUILD_DIR)/ttmwm: $(BUILD_DIR)/libcommon.a $(BUILD_DIR)/x11 $(TTMWM_X11_OBJS) 
	@ echo linking $@
	@ $(CXX) $(CXXFLAGS) -L$(BUILD_DIR) $(TTMWM_X11_OBJS) -lX11 -lcommon -o $@


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
