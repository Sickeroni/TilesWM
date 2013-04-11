#ifndef __CONTAINER_H__
#define __CONTAINER_H__

#include <list>


class Client;
class ContainerContainer;
class ClientContainer;

#if 0
class ContainerElement
{
    friend class ContainerElementList;

public:
    enum Type { CONTAINER, CLIENT }

    ContainerElement *prev();
    ContainerElement *next();

    Type type() { return type; }

private:
    Type _type;
    ContainerElement *_prev, *_next;
};



class ContainerElementList
{
public:
    void prepend(ContainerElement *element);
    void append(ContainerElement *element);
    void insert(ContainerElement *element, ContainerElement *after);

private:
    ContainerElement *_first;
    ContainerElement *_last;
};
#endif


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

    int x() { return _x; }
    int y() { return _y; }
    int width() { return _w; }
    int height() { return _h; }
    bool isHorizontal() {
        return orientation() == HORIZONTAL;
    }
    Orientation orientation();

    bool isContainerContainer();
    bool isClientContainer();

    ContainerContainer *parent() { return _parent; }

    Container *next() { return _next; }
    Container *prev() { return _prev; }


    bool hasChildren();

protected:
    static Container *_root;
    static Orientation _root_orientation;

    Container();

    void local_to_global(int &x, int &y);
    void setParent(ContainerContainer *p) {
        _parent = p;
    }


    ContainerContainer *_parent;
    int _x, _y, _w, _h;

private:

    Container *_prev, *_next;
};

#endif // __CONTAINER_H__
