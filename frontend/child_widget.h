#ifndef __CHILD_WIDGET_H__
#define __CHILD_WIDGET_H__

#include "widget.h"
#include "rect.h"

class Workspace;

class ChildWidget : public Widget
{
public:
    virtual ChildWidget *toChildWidget() override { return this; }

    Widget *parent() { return _parent; }
    void reparent(Widget *parent);

    template <class T>
    T* parentTo() { return dynamic_cast<T*>(_parent); }

    Workspace *workspace();

protected:
    ChildWidget(Type type, WidgetBackend *backend = 0) : Widget(type, backend) {}
    ~ChildWidget() {}

private:
    Widget *_parent = 0;
};

#endif // __CONTAINER_H__
