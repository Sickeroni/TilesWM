include ../make_conf

SOURCES=workspace.cpp container.cpp container_container.cpp client_container.cpp client.cpp list.cpp

all:
	$(CXX) $(CXXFLAGS) -c $(SOURCES)
	ar rcs libfoo.a *.o

clean:
	rm -f *.o
