#ifndef __CONTAINER_H__
#define __CONTAINER_H__

#include "rect.h"

#include "container_base.h"

class Client;
class ContainerLayout;

class Container : public ContainerBase
{
public:
    ~Container();

    virtual int numElements() = 0;
    virtual ClientContainer *activeClientContainer() = 0;
    virtual bool isEmpty() = 0;
    virtual void redrawAll() = 0;
    virtual void handleMaximizedChanged() = 0;
    virtual void handleActiveChanged() = 0;
    virtual ContainerLayout *getLayout() = 0;
    virtual void setMapped(bool mapped) = 0;

    bool isFixedSize() { return _is_fixed_size; }
    void enableFixedSize(bool enable);
    int fixedWidth() { return isMinimized() ? 0 : _fixed_width; }
    void setFixedWidth(int width);
    int fixedHeight() { return isMinimized() ? 0 : _fixed_height; }
    void setFixedHeight(int height);
    void setWorkspace(Workspace *workspace);

    Client *activeClient();

    bool isHorizontal() {
        return orientation() == HORIZONTAL;
    }
    bool isVertical() {
        return orientation() == VERTICAL;
    }

    Orientation orientation();

    ContainerContainer *parent() { return _parent ? _parent->toContainerContainer() : 0; }

    void makeActive();

    Workspace *workspace();
    bool isAncestorOf(Container *container);
    bool isMinimized();
    bool isMaximized();
    bool isActive();

protected:
    Container(Type type);

    ContainerBase *_parent = 0;

private:
    int _fixed_width;
    int _fixed_height;
    bool _is_fixed_size;
};

#endif // __CONTAINER_H__
