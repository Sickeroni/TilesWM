include ../make_conf

COMMON_SRCS :=
# COMMON_SRCS += mouse_handler
COMMON_SRCS += shortcut_set
COMMON_SRCS += config
COMMON_SRCS += string_util
# COMMON_SRCS += container_container_layout
# COMMON_SRCS += client_container_layout
COMMON_SRCS += theme
# COMMON_SRCS += client_util
# COMMON_SRCS += application
# COMMON_SRCS += container_util
# COMMON_SRCS += mode_default
# COMMON_SRCS += mode_3pane
COMMON_SRCS += cairo_canvas
COMMON_SRCS += cairo_icon
# COMMON_SRCS += window_manager_default
# COMMON_SRCS += window_manager_3panel


COMMON_OBJS := $(patsubst %,$(BUILD_DIR)/%.o,$(COMMON_SRCS))


$(OUTPUT_DIR)/libcommon.a: $(COMMON_OBJS)
	@ echo creating static library $@
	@ ar rcs $@ $(COMMON_OBJS)

# pull in dependency info for *existing* .o files
-include $(patsubst %,$(BUILD_DIR)/%.d,$(COMMON_SRCS))

# compile and generate dependency info
$(BUILD_DIR)/%.o: %.cpp
	@ echo $*.cpp
	@ $(CXX) $(CXXFLAGS) -MD -c $*.cpp -o $@
