#ifndef __CONTAINER_H__
#define __CONTAINER_H__

#include "child_widget.h"
#include "rect.h"
#include "common.h"

class Client;
class ContainerLayout;
class ClientContainer;
class ContainerContainer;

class Container : public ChildWidget
{
public:
    enum Type {
        CONTAINER,
        CLIENT,
    };

    enum Orientation {
        HORIZONTAL = 0,
        VERTICAL = 1
    };

    virtual ~Container();

    virtual void handleClick(int x, int y);

    virtual int numElements() const = 0;
    virtual ClientContainer *activeClientContainer() = 0;
    virtual bool isEmpty() const = 0;
    virtual void redrawAll() = 0;
    virtual ContainerLayout *getLayout() = 0;
    virtual bool isMinimized() const = 0;

    bool isFixedSize() const { return _is_fixed_size; }
    void enableFixedSize(bool enable);
    int fixedWidth() const { return isMinimized() ? 0 : _fixed_width; }
    void setFixedWidth(int width);
    int fixedHeight() const { return isMinimized() ? 0 : _fixed_height; }
    void setFixedHeight(int height);

    bool isActive() {
        UNIMPLEMENTED
        return false;
    }

    Client *activeClient();

    bool isHorizontal() {
        return orientation() == HORIZONTAL;
    }
    bool isVertical() {
        return orientation() == VERTICAL;
    }
    Orientation orientation() { return _orientation; }

    ContainerContainer *parentContainer();

    bool isAncestorOf(Container *container) const;

    Type type() const { return _type; }
    bool isContainerContainer() const { return _type == CONTAINER; }
    bool isClientContainer() const { return _type == CLIENT; }
    ClientContainer *toClientContainer();
    ContainerContainer *toContainerContainer();

protected:
    Container(Type type);

private:
    const Type _type;
    int _fixed_width = 0;
    int _fixed_height = 0;
    bool _is_fixed_size = false;
    Orientation _orientation = HORIZONTAL;
};

#endif // __CONTAINER_H__
