#ifndef __WIDGET_H__
#define __WIDGET_H__

#include "rect.h"

class WidgetBackend;
class Client;
class Container;
class Workspace;
class ChildWidget;

class Widget
{
public:
    enum Type {
        CLIENT,
        CONTAINER,
        WORKSPACE,
        OTHER
    };

    virtual ChildWidget *toChildWidget() { return 0; }

    const Rect &rect() const { return _rect; }
    void setRect(const Rect &rect);
    int width() const { return _rect.w; }
    int height() const { return _rect.h; }
    void setMapped(bool mapped);
    void redraw();
    int maxTextHeight() const;
    Client *toClient();
    Workspace *toWorkspace();
    Container *toContainer();

protected:
    Widget(Type type) : _type(type) {}
    ~Widget() {}

    WidgetBackend *_backend = 0;
    const Type _type;
    Rect _rect;
};

#endif
