#ifndef __CONTAINER_H__
#define __CONTAINER_H__

#include "list.h"
#include "rect.h"

class Client;
class ContainerContainer;
class ClientContainer;
class Workspace;


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

    static void rotateOrientation();

    static bool isForwardDirection(Direction dir) {
        return (dir == EAST || dir == SOUTH);
    }

    virtual ~Container() {}


    bool isFixedSize() { return _is_fixed_size; }
    void enableFixedSize(bool enable);

    virtual int numElements() = 0;
    virtual ClientContainer *activeClientContainer() = 0;
    virtual void layout() = 0;
//     virtual void layoutClients();
    virtual bool isEmpty() = 0;
    virtual void redrawAll() = 0;
    virtual void handleMaximizedChanged() = 0;
    virtual void handleActiveChanged() = 0;



    int minWidth();
    int maxWidth();
    int minHeight();
    int maxHeight();
    int fixedWidth() { return isMinimized() ? 0 : _fixed_width; }
    void setFixedWidth(int width);
    int fixedHeight() { return isMinimized() ? 0 : _fixed_height; }
    void setFixedHeight(int height);

    virtual void reparent(ContainerContainer *p) {
        _parent = p;
    }
    virtual void setRect(const Rect &rect);

    int x() { return _rect.x; }
    int y() { return _rect.y; }
    int width() { return _rect.w; }
    int height() { return _rect.h; }
    const Rect &rect() { return _rect; }

    bool isHorizontal() {
        return orientation() == HORIZONTAL;
    }
    Orientation orientation();

    Type type() { return _type; }
    bool isContainerContainer() { return _type == CONTAINER; }
    bool isClientContainer() { return _type == CLIENT; }

    ContainerContainer *parent() { return _parent; }

//     void makeActive(); //FIXME change to makeActiveAndFocus() ?

    ContainerContainer *root();

    Workspace *workspace() { return _workspace; }
    bool isAncestorOf(Container *container);
    bool isMinimized();
    bool isActive();


protected:
    virtual int minWidthInt() = 0;
    virtual int maxWidthInt() = 0;
    virtual int minHeightInt() = 0;
    virtual int maxHeightInt() = 0;

    Container(Type type, ContainerContainer *parent);

    void localToGlobal(int &x, int &y);
    void globalToLocal(int &x, int &y);
    void setWorkspace(Workspace *workspace);

    static Container *_root;
    static Orientation _root_orientation;

    ContainerContainer *_parent;
    Rect _rect;

private:
    const Type _type;
    Workspace *_workspace;
    int _fixed_width;
    int _fixed_height;
    bool _is_fixed_size;
};

#endif // __CONTAINER_H__
