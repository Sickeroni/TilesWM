include ../config.mk

BUILD_DIR := ../build/tests

DEFAULT_TARGETS :=
DEFAULT_TARGETS += $(BUILD_DIR)/x11_backend_test

.PHONY : all
all: $(DEFAULT_TARGETS)

$(BUILD_DIR):
	@ mkdir -p $@

$(BUILD_DIR)/x11_backend_test: $(BUILD_DIR) $(BUILD_DIR)/x11_backend_test.o
	@ echo linking $@
	@ $(CXX) $(CXXFLAGS) -L../build -L$(BUILD_DIR) $(BUILD_DIR)/x11_backend_test.o -lX11 -lXcursor -lbackend_x11 -lbackend_x11_native -lcommon -o $@


# pull in dependency info for *existing* .o files
-include $(BUILD_DIR)/x11_backend_test.d

# compile and generate dependency info
$(BUILD_DIR)/%.o: %.cpp
	@ echo $*.cpp
	@ $(CXX) $(CXXFLAGS) -MD -I../common -c $*.cpp -o $@


.PHONY : clean
clean:
	rm -rf $(BUILD_DIR)/*
