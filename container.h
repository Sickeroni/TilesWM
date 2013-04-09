#ifndef __CONTAINER_H__
#define __CONTAINER_H__

#include <list>


class Client;
class ContainerContainer;
class ClientContainer;


class Container
{
public:
    enum Orientation {
        HORIZONTAL = 0,
        VERTICAL = 1
    };

    enum Direction {
        WEST,
        EAST,
        NORTH,
        SOUTH
    };

    enum MaxDimension {
        MAX_DIMENSION = 8
    };

    static Orientation orientationOfDirection(Direction dir) {
        if (dir == WEST || dir == EAST)
            return HORIZONTAL;
        else
            return VERTICAL;
    }

//     static Orientation defaultOrientation() {
//         return HORIZONTAL;
//     }

    static void startup(int screen_width, int screen_height);
    static void shutdown();
    static Container *root() {
        return _root;
    }

    virtual ClientContainer *currentClientContainer() = 0;
    virtual void addClient(Client *c) = 0;
    virtual void layout() = 0;

    int x() { return _x; }
    int y() { return _y; }
    int width() { return _w; }
    int height() { return _h; }
    bool isHorizontal() {
        return orientation() == HORIZONTAL;
    }
    Orientation orientation();
//     bool isClientContainer();

    ContainerContainer *parent() { return _parent; }
    void setParent(ContainerContainer *p) {
        _parent = p;
    }
    bool isUnlinked() {
        return !_prev && !_next;
    }
    Container *next() { return _next; }
    Container *prev() { return _prev; }
    void append (Container *container);

protected:
    Container(ContainerContainer *parent, int x, int y, int w, int h);

    void local_to_global(int &x, int &y);


    static Container *_root;
    static Orientation _root_orientation;

    ContainerContainer *_parent;
    int _x, _y, _w, _h;

private:

    Container *_prev, *_next;
};

#endif // __CONTAINER_H__
