include ../make_conf

OBJS := \
workspace.o \
container.o \
container_container.o \
client_container.o \
client.o \
list.o

all: libcommon.a
# 	echo objects: $(OBJS)

libcommon.a: $(OBJS)
	ar rcs libcommon.a $(OBJS)


# pull in dependency info for *existing* .o files
# from gnu make manual:
# If you want make to simply ignore a makefile which does not exist or cannot be remade,
# with no error message, use the -include directive instead of include
-include $(OBJS:.o=.d)

# compile and generate dependency info
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MD -c $*.cpp


clean:
	rm -f libcommon.a *.o *.d
