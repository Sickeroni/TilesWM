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

    virtual int numElements() = 0;
    virtual void setFocus() = 0;
    virtual ClientContainer *activeClientContainer() = 0;
    virtual void addClient(Client *c) = 0;
    virtual void layout() = 0;
//     virtual void layoutClients();
    virtual bool isEmpty() = 0;
    virtual void redrawAll() = 0;
    virtual void handleMaximizedChanged() = 0;
    virtual void handleActiveChanged() = 0;


//     int minSize();
//     int maxSize();
//     int size();
//     double availableSpacePortion() { return _available_space_portion; }
//     void setAvailableSpacePortion(double portion) {
//         _available_space_portion = portion;
//     }

//     void incAvailableSpacePortion(int pixels);
//     void decAvailableSpacePortion(int pixels);
//     int extraSize() { return _custom_size; }
//     void setCustomSizeActive(bool active);

    int minWidth();
    int maxWidth();
    int minHeight();
    int maxHeight();
    int fixedWidth() { return _fixed_width; }
    void setFixedWidth(int width);
//     void changeFixedWidth(int delta);
    int fixedHeight() { return _fixed_height; }
    void setFixedHeight(int height);
//     void changeFixedHaight(int delta);

    virtual void reparent(ContainerContainer *p) {
        _parent = p;
    }
    virtual void setRect(const Rect &rect);
    // FIXME UGLY
//     virtual int extraSpace() { return 0; }

    int x() { return _rect.x; }
    int y() { return _rect.y; }
    int width() { return _rect.w; }
    int height() { return _rect.h; }
    const Rect &rect() { return _rect; }
//     virtual void setRect(int x, int y, int w, int h);

    bool isHorizontal() {
        return orientation() == HORIZONTAL;
    }
    Orientation orientation();

    Type type() { return _type; }
    bool isContainerContainer() { return _type == CONTAINER; }
    bool isClientContainer() { return _type == CLIENT; }

    ContainerContainer *parent() { return _parent; }

    bool hasFocus();

    void makeActive(); //FIXME change to makeActiveAndFocus() ?

    ContainerContainer *root();

    Workspace *workspace() { return _workspace; }
    bool isAncestorOf(Container *container);
    bool isMinimized();


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
    // reserved portion of available space
    double _available_space_portion;
    int _fixed_width;
    int _fixed_height;
};

#endif // __CONTAINER_H__
