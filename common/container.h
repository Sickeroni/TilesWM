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

    Client *activeClient();

    bool isHorizontal() {
        return orientation() == HORIZONTAL;
    }
    bool isVertical() {
        return orientation() == VERTICAL;
    }

    Orientation orientation();

    //FIXME
    // should just return _parent
    ContainerContainer *parent() { return _parent ? _parent->toContainerContainer() : 0; }
    //FIXME remove
    bool hasParent() { return _parent != 0; }

    void reparent(ContainerBase *parent, ContainerWidget *parent_widget);

    void makeActive();

    Workspace *workspace();
    bool isAncestorOf(Container *container) const;
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
