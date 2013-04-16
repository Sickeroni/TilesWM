CXXFLAGS?=-pedantic -Wall -g #-Werror

SOURCES=x11_container_container.cpp x11_widget.cpp x11_client_widget.cpp x11_client.cpp x11_application.cpp main.cpp

all:
	$(CXX) $(CXXFLAGS) -I../common -L../common -lX11 -o ttm-wm $(SOURCES)

clean:
	rm -f ttm-wm
