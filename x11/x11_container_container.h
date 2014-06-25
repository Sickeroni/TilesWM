#ifndef __X11_CONTAINER_CONTAINER_H__
#define __X11_CONTAINER_CONTAINER_H__

#include "container_container.h"

#include "x11_server_widget.h"

#include <vector>

class Workspace;

class X11ContainerContainer final : public ContainerContainer, public X11ServerWidget::EventHandler
{
public:
    X11ContainerContainer();
    ~X11ContainerContainer();

    virtual int numElements() override { return _children.size(); }
    virtual Container *child(int index) override { return _children[index]; }
    virtual bool isEmpty() override { return _children.empty(); }
    virtual int activeChildIndex() override { return _active_child_index; }
    virtual int indexOfChild(const Container *child) override;
    virtual void setActiveChild(int index) override;
    virtual int addChild(Container *child) override;
    virtual void insertChild(Container *child, int insert_pos) override;
    virtual Container *replaceChild(int index, Container *new_child) override;
    virtual Container *removeChild(int index) override;
    virtual void setRect(const Rect &rect) override;
    virtual int maxTextHeight() const override;
    virtual void redraw() override;
    virtual void setMapped(bool mapped) override;

    // X11ServerWidget::EventHandler implementaion
    virtual void handleExpose() override {
        redraw();
    }

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
