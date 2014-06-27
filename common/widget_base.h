#ifndef __WIDGET__BASE_H__
#define __WIDGET__BASE_H__

#include "rect.h"

class WidgetBase
{
public:
    enum Type {
        CLIENT,
        CONTAINER
    };

    virtual ~WidgetBase() {}

    Type type() { return _type; }

    virtual void setRect(const Rect &rect) = 0;
    virtual void setMapped(bool mapped) = 0;

protected:
    WidgetBase(Type type) : _type(type) {}

private:
    const Type _type;
};

#endif
