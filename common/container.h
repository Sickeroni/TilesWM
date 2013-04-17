#ifndef __CONTAINER_H__
#define __CONTAINER_H__

#include "list.h"
#include "rect.h"

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

//     enum MaxDimension {
//         MAX_DIMENSION = 8
//     };

    static Orientation orientationOfDirection(Direction dir) {
        if (dir == WEST || dir == EAST)
            return HORIZONTAL;
        else
            return VERTICAL;
    }

    static bool isForwardDirection(Direction dir) {
        return (dir == EAST || dir == SOUTH);
    }

    virtual ~Container() {}

    virtual ClientContainer *activeClientContainer() = 0;
    virtual void addClient(Client *c) = 0;
    virtual void layout() = 0;
//     virtual void layoutClients();
    virtual bool isEmpty() = 0;

    virtual void reparent(ContainerContainer *p) {
        _parent = p;
    }
    virtual void setRect(const Rect &rect);

    int x() { return _rect.x; }
    int y() { return _rect.y; }
    int width() { return _rect.w; }
    int height() { return _rect.h; }
//     virtual void setRect(int x, int y, int w, int h);

    bool isHorizontal() {
        return orientation() == HORIZONTAL;
    }
    Orientation orientation();

    Type type() { return _type; }
    bool isContainerContainer() { return _type == CONTAINER; }
    bool isClientContainer() { return _type == CLIENT; }

    ContainerContainer *parent() { return _parent; }


protected:
    static Container *_root;
    static Orientation _root_orientation;

    Container(Type type, ContainerContainer *parent);

    void localToGlobal(int &x, int &y);


    ContainerContainer *_parent;
    Rect _rect;

private:
    const Type _type;
};

#endif // __CONTAINER_H__
