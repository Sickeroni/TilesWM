PREFIX?=/usr/X11R6
CXXFLAGS?=-pedantic -Wall -g #-Werror

SOURCES=list.cpp container.cpp container_container.cpp client_container.cpp client.cpp main.cpp
#SOURCES=main.cpp
#SOURCES=list.cpp

all:
	$(CXX) $(CXXFLAGS) -I$(PREFIX)/include -L$(PREFIX)/lib -lX11 -o ttm-wm $(SOURCES)

clean:
	rm -f ttm-wm
