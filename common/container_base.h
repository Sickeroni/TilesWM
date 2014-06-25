#ifndef __CONTAINER_BASE_H__
#define __CONTAINER_BASE_H__

#include "rect.h"

class Container;
class ContainerContainer;
class ClientContainer;
class Workspace;

class ContainerBase
{
public:
    enum Type {
        CONTAINER,
        CLIENT,
        WORKSPACE
    };

    enum Orientation {
        HORIZONTAL = 0,
        VERTICAL = 1
    };

    virtual ~ContainerBase() {}

    virtual void setRect(const Rect &rect) {
        _rect.set(rect);
    }

    Type type() { return _type; }
    bool isContainerContainer() { return _type == CONTAINER; }
    bool isClientContainer() { return _type == CLIENT; }
    bool isWorkspace() { return _type == WORKSPACE; }
    bool isContainer() { return !isWorkspace(); }
    ClientContainer *toClientContainer();
    ContainerContainer *toContainerContainer();
    Workspace *toWorkspace();

    const Rect &rect() { return _rect; }
    int width() { return _rect.w; }
    int height() { return _rect.h; }

protected:
    ContainerBase(Type type) : _type(type) {}

private:
    const Type _type;
    Rect _rect;
};

#endif
