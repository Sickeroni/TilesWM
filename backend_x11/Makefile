include ../make_conf

BACKEND_X11_SRCS :=
BACKEND_X11_SRCS += x11_widget_backend
BACKEND_X11_SRCS += x11_widget
BACKEND_X11_SRCS += x11_server_widget
BACKEND_X11_SRCS += x11_client_widget
BACKEND_X11_SRCS += x11_client
BACKEND_X11_SRCS += x11_shortcut_set
BACKEND_X11_SRCS += x11_application
BACKEND_X11_SRCS += x11_global

BACKEND_X11_OBJS := $(patsubst %,$(BUILD_DIR)/%.o,$(BACKEND_X11_SRCS))

$(OUTPUT_DIR)/libbackend_x11.a: $(BACKEND_X11_OBJS)
	@ echo creating static library $@
	@ ar rcs $@ $(BACKEND_X11_OBJS)

# pull in dependency info for *existing* .o files
-include $(patsubst %,$(BUILD_DIR)/%.d,$(BACKEND_X11_OBJS))

# compile and generate dependency info
$(BUILD_DIR)/%.o: %.cpp
	@ echo $*.cpp
	@ $(CXX) $(CXXFLAGS) -MD -I../backend_x11_common -I../common -c $*.cpp -o $@
