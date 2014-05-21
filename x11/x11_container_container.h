#ifndef __X11_CONTAINER_CONTAINER_H__
#define __X11_CONTAINER_CONTAINER_H__

#include "container_container.h"

#include "x11_server_widget.h"

#include <vector>

class Workspace;

class X11ContainerContainer : public ContainerContainer, public X11ServerWidget::EventHandler
{
#if 0
    ContainerContainer *createContainerContainer()
    {
        ContainerContainer *c = new ContainerContainer(this);
        X11ServerWidget *w = new X11ServerWidget(_widget);
    };

#endif

public:
    static X11ContainerContainer *create(Workspace *workspace);

    virtual ~X11ContainerContainer();

    
    virtual void redraw();
    virtual int numElements() { return _children.size(); }
    virtual Container *child(int index) { return _children[index]; }
    virtual bool isEmpty() { return _children.empty(); }
    virtual void setActiveChild(int index);
    virtual int activeChildIndex() { return _active_child_index; }


    // X11ServerWidget::EventHandler implementaion
    virtual void handleExpose() {
        redraw();
    }


    virtual void setRect(const Rect &rect);
    virtual void reparent(ContainerContainer *p);
    virtual void deleteEmptyChildren();
//     virtual void setFocus();

    X11ServerWidget *widget() { return _widget; }
    

// protected:
//     virtual ContainerContainer *createContainerContainer();
//     virtual ClientContainer *createClientContainer();

private:
    X11ContainerContainer(Workspace *workspace, X11ContainerContainer *parent);
    void clear();
    void deleteChild(Container *child);

    std::vector<Container*> _children;
    int _active_child_index;
    X11ServerWidget *_widget;
};

#endif // __X11_CONTAINER_CONTAINER_H__
