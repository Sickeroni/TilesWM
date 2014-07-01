include ../make_conf

BACKEND_X11_NATIVE_SRCS :=
BACKEND_X11_NATIVE_SRCS += x11_canvas
BACKEND_X11_NATIVE_SRCS += x11_icon
BACKEND_X11_NATIVE_SRCS += x11_graphics_system_x11

BACKEND_X11_NATIVE_OBJS := $(patsubst %,$(BUILD_DIR)/%.o,$(BACKEND_X11_NATIVE_SRCS))


$(OUTPUT_DIR)/libbackend_x11_native.a: $(BACKEND_X11_NATIVE_OBJS)
	@ echo creating static library $@
	@ ar rcs $@ $(BACKEND_X11_NATIVE_OBJS)

# pull in dependency info for *existing* .o files
-include $(patsubst %,$(BUILD_DIR)/%.d,$(BACKEND_X11_NATIVE_OBJS))

# compile and generate dependency info
$(BUILD_DIR)/%.o: %.cpp
	@ echo $*.cpp
	@ $(CXX) $(CXXFLAGS) -MD -I../backend_x11_common -I../common -c $*.cpp -o $@
