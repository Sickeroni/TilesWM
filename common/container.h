#ifndef __CONTAINER_H__
#define __CONTAINER_H__

#include "rect.h"

class Client;
class ContainerContainer;
class ClientContainer;
class Workspace;
class ContainerLayout;

class Container
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

    virtual ~Container() {}

    virtual int numElements() = 0;
    virtual ClientContainer *activeClientContainer() = 0;
    virtual bool isEmpty() = 0;
    virtual void redrawAll() = 0;
    virtual void handleMaximizedChanged() = 0;
    virtual void handleActiveChanged() = 0;
    virtual ContainerLayout *getLayout() = 0;
    virtual void setMapped(bool mapped) = 0;

    virtual void setRect(const Rect &rect);

    bool isFixedSize() { return _is_fixed_size; }
    void enableFixedSize(bool enable);
    int fixedWidth() { return isMinimized() ? 0 : _fixed_width; }
    void setFixedWidth(int width);
    int fixedHeight() { return isMinimized() ? 0 : _fixed_height; }
    void setFixedHeight(int height);

    Client *activeClient();

    int x() { return _rect.x; }
    int y() { return _rect.y; }
    int width() { return _rect.w; }
    int height() { return _rect.h; }
    const Rect &rect() { return _rect; }
    bool isHorizontal() {
        return orientation() == HORIZONTAL;
    }
    bool isVertical() {
        return orientation() == VERTICAL;
    }

    Orientation orientation();

    Type type() { return _type; }
    bool isContainerContainer() { return _type == CONTAINER; }
    bool isClientContainer() { return _type == CLIENT; }

    ContainerContainer *parent() { return _parent; }

    void makeActive();

    Workspace *workspace();
    bool isAncestorOf(Container *container);
    bool isMinimized();
    bool isMaximized();
    bool isActive();

    void setWorkspace(Workspace *workspace);

    static void rotateOrientation();

protected:
    Container(Type type);

    void localToGlobal(int &x, int &y);
    void globalToLocal(int &x, int &y);

    static Container *_root;
    static Orientation _root_orientation;

    ContainerContainer *_parent;
    Workspace *_workspace;
    Rect _rect;

private:
    const Type _type;
    int _fixed_width;
    int _fixed_height;
    bool _is_fixed_size;
};

#endif // __CONTAINER_H__
