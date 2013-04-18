CXXFLAGS?=-pedantic -Wall -g #-Werror

SOURCES=x11_canvas.cpp x11_container_container.cpp x11_client_container.cpp x11_widget.cpp x11_server_widget.cpp x11_client_widget.cpp x11_client.cpp x11_application.cpp main.cpp

all: ttm-wm

common:
	(cd ../common && make)

ttm-wm: common
	$(CXX) $(CXXFLAGS) -I../common -lX11 -o ttm-wm $(SOURCES) ../common/*.o


clean:
	rm -f ttm-wm
