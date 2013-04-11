PREFIX?=/usr/X11R6
CXXFLAGS?=-pedantic -Wall #-g #-Werror

SOURCES=container_element_list.cpp container.cpp container_container.cpp client_container.cpp client.cpp main.cpp
#SOURCES=main.cpp
# SOURCES=container_element_list.cpp

all:
	$(CXX) $(CXXFLAGS) -I$(PREFIX)/include -L$(PREFIX)/lib -lX11 -o ttm-wm $(SOURCES)

clean:
	rm -f ttm-wm
