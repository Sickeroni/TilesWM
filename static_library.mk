include $(TOP_DIR)/config.mk

OBJ_DIR := $(BUILD_DIR)/$(LIBNAME)
OBJS := $(patsubst %,$(OBJ_DIR)/%.o,$(SRCS))
DEP_FILES := $(patsubst %,$(OBJ_DIR)/%.d,$(SRCS))
OUTPUT_FILE := $(BUILD_DIR)/lib$(LIBNAME).a

.PHONY : all
all: $(OUTPUT_FILE)

$(OBJ_DIR):
	@ mkdir -p $@

$(OUTPUT_FILE): $(OBJ_DIR) $(OBJS)
	@ echo creating static library $@
	@ ar rcs $@ $(OBJS)

# pull in dependency info for *existing* .o files
-include $(DEP_FILES)

# compile and generate dependency info
$(OBJ_DIR)/%.o: %.cpp
	@ echo $*.cpp
	@ $(CXX) $(CXXFLAGS) -MD -c $*.cpp -o $@

.PHONY : clean
clean:
	rm -f $(OBJ_DIR)/*
	rm -f $(OUTPUT_FILE)
