#ifndef __CONTAINER_BASE_H__
#define __CONTAINER_BASE_H__

#include "rect.h"

class Container;
class ContainerContainer;
class ClientContainer;
class Workspace;
class ContainerWidget;

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

    virtual ~ContainerBase();

    void setRect(const Rect &rect);

    Type type() const { return _type; }
    bool isContainerContainer() const { return _type == CONTAINER; }
    bool isClientContainer() const { return _type == CLIENT; }
    bool isWorkspace() const { return _type == WORKSPACE; }
    bool isContainer() const { return !isWorkspace(); }
    ClientContainer *toClientContainer();
    ContainerContainer *toContainerContainer();
    Workspace *toWorkspace();

    const Rect &rect() const { return _rect; }
    int width() const { return _rect.w; }
    int height() const { return _rect.h; }
    void setMapped(bool mapped);
    void redraw();
    const ContainerWidget *widget() const { return _widget; }
    int maxTextHeight() const;

protected:
    ContainerBase(Type type);

    ContainerWidget *widget() { return _widget; }

private:
    const Type _type;
    Rect _rect;
    ContainerWidget *_widget = 0;
};

#endif
