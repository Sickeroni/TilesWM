OBJS := $(patsubst %,$(BUILD_DIR)/%.o,$(SRCS))

$(OUTPUT_DIR)/lib%.a: $(OBJS)
	@ echo creating static library $@
	@ ar rcs $@ $(OBJS)

# pull in dependency info for *existing* .o files
-include $(patsubst %,$(BUILD_DIR)/%.d,$(OBJS))

# compile and generate dependency info
$(BUILD_DIR)/%.o: %.cpp
	@ echo $*.cpp
	@ $(CXX) $(CXXFLAGS) -MD -c $*.cpp -o $@
