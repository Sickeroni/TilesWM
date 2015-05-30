#ifndef __CHILD_WIDGET_H__
#define __CHILD_WIDGET_H__

#include "widget.h"
#include "rect.h"

class Workspace;

class ChildWidget : public Widget
{
public:
//     ~ChildWidget();

    virtual ChildWidget *toChildWidget() override { return this; }

//     virtual int numElements() const = 0;
//     virtual bool isEmpty() const = 0;
//     virtual ContainerLayout *getLayout() = 0;
//     virtual bool isMinimized() const = 0;

#if 0
    bool isFixedSize() const { return _is_fixed_size; }
    void enableFixedSize(bool enable);
    int fixedWidth() const { return isMinimized() ? 0 : _fixed_width; }
    void setFixedWidth(int width);
    int fixedHeight() const { return isMinimized() ? 0 : _fixed_height; }
    void setFixedHeight(int height);
#endif

//     Client *activeClient();

#if 0
    bool isHorizontal() {
        return orientation() == HORIZONTAL;
    }
    bool isVertical() {
        return orientation() == VERTICAL;
    }

    Orientation orientation();
#endif

    Widget *parent() { return _parent; }
    void reparent(Widget *parent);

    template <class T>
    T* parentTo() { return dynamic_cast<T*>(_parent); }

    Workspace *workspace();
//     bool isAncestorOf(ChildWidget *container) const;

//     bool isFloating() { return _is_floating; }

protected:
    ChildWidget(Type type, WidgetBackend *backend = 0) : Widget(type, backend) {}
    ~ChildWidget() {}

//     bool _is_floating = false;

private:
    Widget *_parent = 0;
//     int _fixed_width;
//     int _fixed_height;
//     bool _is_fixed_size;
};

#endif // __CONTAINER_H__
