include make_conf


LIBCOMMON_SRCS := \
config \
string_util \
container_container_layout \
client_container_layout \
theme \
workspace \
container \
container_container \
client_container \
client \
monitor \
application \
actions \
container_util \
mode \
mode_default \
mode_3panel

TTMWM_X11_SRCS := \
x11_icon \
x11_canvas \
x11_container_container \
x11_client_container \
x11_widget \
x11_server_widget \
x11_client_widget \
x11_client \
x11_shortcut \
x11_shortcut_set \
x11_application \
x11_global \
main


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
