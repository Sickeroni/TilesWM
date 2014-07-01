#ifndef __WIDGET_FRONTEND_FRONTEND__
#define __WIDGET_FRONTEND_FRONTEND__

class Canvas;

class WidgetFrontend
{
public:
    virtual void draw(Canvas *canvas) = 0;
};

#endif
