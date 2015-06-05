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

    virtual void setRect(const Rect &rect);
    virtual ChildWidget *toChildWidget() { return 0; }
    virtual bool isMinimized() const { return _is_minimized; }

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
    void setMinimized(bool minimized);
    void grabMouse(WidgetBackend::CursorType cursor) {
        _backend->grabMouse(cursor);
    }
    void releaseMouse() {
        _backend->releaseMouse();
    }

    template <class T>
    T* to() { return dynamic_cast<T*>(this); }

protected:
    Widget(Type type, WidgetBackend *backend = 0);
    ~Widget();

    void reparent(Widget *parent);

private:
    WidgetBackend *_backend = 0;
    bool _owns_backend = false;
    bool _is_minimized = false;
    const Type _type;
    Rect _rect;
};

#endif
