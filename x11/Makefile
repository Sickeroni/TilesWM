include ../make_conf

OBJS := \
x11_icon.o \
x11_canvas.o \
x11_container_container.o \
x11_client_container.o \
x11_widget.o \
x11_server_widget.o \
x11_client_widget.o \
x11_client.o x11_shortcut.o \
x11_shortcut_set.o \
x11_default_key_bindings.o \
x11_application.o \
x11_global.o \
main.o

# SOURCES=x11_icon.cpp x11_canvas.cpp x11_container_container.cpp x11_client_container.cpp x11_widget.cpp x11_server_widget.cpp x11_client_widget.cpp x11_client.cpp x11_shortcut.cpp x11_shortcut_set.cpp x11_default_key_bindings.cpp x11_application.cpp x11_global.cpp main.cpp

all: ttm-wm

common:
	(cd ../common && make)

ttm-wm: common $(OBJS)
	$(CXX) $(CXXFLAGS) -L../common -lfoo -lX11 $(OBJS) -o ttm-wm --verbose

# pull in dependency info for *existing* .o files
# from gnu make manual:
# If you want make to simply ignore a makefile which does not exist or cannot be remade,
# with no error message, use the -include directive instead of include
-include $(OBJS:.o=.d)

# compile and generate dependency info
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -I../common -MD -c $*.cpp

clean:
	rm -f ttm-wm *.o .*.d
