include make_conf

BUILD_DIR=build

DEFAULT_TARGETS :=
DEFAULT_TARGETS += $(BUILD_DIR)/ttmwm
# DEFAULT_TARGETS += $(BUILD_DIR)/ttmwm-cairo

.PHONY : all
all: $(DEFAULT_TARGETS)

$(BUILD_DIR)/lib%.a: %/*.cpp %/*.h
	(mkdir -p $(BUILD_DIR)/$* && cd $* &&  make BUILD_DIR=../$(BUILD_DIR)/$* OUTPUT_DIR=../$(BUILD_DIR) ../$(BUILD_DIR)/lib$*.a)

$(BUILD_DIR)/ttmwm: $(BUILD_DIR)/libcommon.a $(BUILD_DIR)/libbackend_x11.a $(BUILD_DIR)/libbackend_x11_native.a $(BUILD_DIR)/ttmwm.o
	@ echo linking $@
	@ $(CXX) $(CXXFLAGS) -L$(BUILD_DIR) $(BUILD_DIR)/ttmwm.o -lX11 -lXcursor -lbackend_x11 -lbackend_x11_native -lcommon -o $@

$(BUILD_DIR)/ttmwm-cairo: $(BUILD_DIR)/libcommon.a $(BUILD_DIR)/x11 $(TTMWM_X11_OBJS) $(BUILD_DIR)/x11/x11_graphics_system_cairo.o
	@ echo linking $@
	@ $(CXX) $(CXXFLAGS) -L$(BUILD_DIR) $(TTMWM_X11_OBJS) $(BUILD_DIR)/x11/x11_graphics_system_cairo.o -lX11 -lXcursor -lcairomm-1.0 -lcommon -o $@


# pull in dependency info for *existing* .o files
-include $(BUILD_DIR)/ttmwm.d

# compile and generate dependency info
$(BUILD_DIR)/%.o: %.cpp
	@ echo $*.cpp
	@ $(CXX) $(CXXFLAGS) -MD -Icommon -c $*.cpp -o $@

.PHONY : doc
doc:
	BUILD_DIR=$(BUILD_DIR) doxygen Doxyfile
	BUILD_DIR=$(BUILD_DIR) doxygen Doxyfile_common

.PHONY : clean
clean: 
	rm -rf $(BUILD_DIR)/*
