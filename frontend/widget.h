#ifndef __WIDGET_H__
#define __WIDGET_H__

#include "widget_frontend.h"
#include "widget_backend.h"
#include "rect.h"
#include "common.h"

class Client;
class Workspace;
class ChildWidget;

class Widget : public WidgetFrontend
{
public:
    enum Type {
        WORKSPACE,
        OTHER
    };

    virtual void draw(Canvas *canvas) override;
    virtual void setRect(const Rect &rect);
    virtual ChildWidget *toChildWidget() { return 0; }

    const Rect &rect() const { return _rect; }
    int width() const { return _rect.w; }
    int height() const { return _rect.h; }
    void setMapped(bool mapped);
    void redraw();
    int maxTextHeight() const;
    Workspace *toWorkspace();
    void raise() {
        _backend->raise();
    }
    void globalToLocal(int &x, int &y);

    template <class T>
    T* to() { return dynamic_cast<T*>(this); }

protected:
    Widget(Type type) : _type(type) {}
    ~Widget() {}

    WidgetBackend *_backend = 0;
    const Type _type;
    Rect _rect;
};

#endif
