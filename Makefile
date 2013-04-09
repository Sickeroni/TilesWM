PREFIX?=/usr/X11R6
CXXFLAGS?=-pedantic -Wall -g #-Werror

SOURCES=container.cpp client.cpp main.cpp

all:
	$(CXX) $(CXXFLAGS) -I$(PREFIX)/include -L$(PREFIX)/lib -lX11 -o ttm-wm $(SOURCES)

clean:
	rm -f ttm-wm
