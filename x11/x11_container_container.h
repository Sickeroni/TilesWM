#ifndef __X11_CONTAINER_CONTAINER_H__
#define __X11_CONTAINER_CONTAINER_H__

#include "container_container.h"

#include "x11_server_widget.h"

#include <vector>

class Workspace;

class X11ContainerContainer : public ContainerContainer, public X11ServerWidget::EventHandler
{
public:
    X11ContainerContainer();
    virtual ~X11ContainerContainer();


    virtual void setMapped(bool mapped);
    virtual void redraw();
    virtual int numElements() { return _children.size(); }
    virtual Container *child(int index) { return _children[index]; }
    virtual bool isEmpty() { return _children.empty(); }
    virtual int activeChildIndex() { return _active_child_index; }
    virtual int indexOfChild(const Container *child);
    virtual void setActiveChild(int index);
    virtual int addChild(Container *child);
    virtual void insertChild(Container *child, int insert_pos);
    virtual Container *replaceChild(int index, Container *new_child);
    virtual Container *removeChild(int index);

    // X11ServerWidget::EventHandler implementaion
    virtual void handleExpose() {
        redraw();
    }


    virtual void setRect(const Rect &rect);
//     virtual void setFocus();

    void reparent(X11ContainerContainer *p);

    X11ServerWidget *widget() { return _widget; }

private:
    void clear();

    // helper
    static void reparentContainer(Container *container, X11ContainerContainer *parent);

    std::vector<Container*> _children;
    int _active_child_index;
    X11ServerWidget *_widget;
};

#endif // __X11_CONTAINER_CONTAINER_H__
