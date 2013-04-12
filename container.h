#ifndef __CONTAINER_H__
#define __CONTAINER_H__

#include "list.h"


class Client;
class ContainerContainer;
class ClientContainer;


class Container : public List<Container>::Item
{
public:
    enum Type {
        CONTAINER,
        CLIENT
    };

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

    static bool isForwardDirection(Direction dir) {
        return (dir == EAST || dir == SOUTH);
    }

    static void startup(int screen_width, int screen_height);
    static void shutdown();
    static Container *root() {
        return _root;
    }

    virtual ClientContainer *activeClientContainer() = 0;
    virtual void addClient(Client *c) = 0;
    virtual void layout() = 0;
//     virtual void layoutClients();
    virtual bool isEmpty() = 0;

    int x() { return _x; }
    int y() { return _y; }
    int width() { return _w; }
    int height() { return _h; }
    bool isHorizontal() {
        return orientation() == HORIZONTAL;
    }
    Orientation orientation();

    Type type() { return _type; }
    bool isContainerContainer() { return _type == CONTAINER; }
    bool isClientContainer() { return _type == CLIENT; }

    ContainerContainer *parent() { return _parent; }
    void setParent(ContainerContainer *p) {
        _parent = p;
    }


protected:
    static Container *_root;
    static Orientation _root_orientation;

    Container(Type type);

    void localToGlobal(int &x, int &y);


    ContainerContainer *_parent;
    int _x, _y, _w, _h;

private:
    const Type _type;
};

#endif // __CONTAINER_H__
