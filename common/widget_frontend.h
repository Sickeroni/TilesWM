#ifndef __WIDGET_FRONTEND_FRONTEND__
#define __WIDGET_FRONTEND_FRONTEND__

class Canvas;

class WidgetFrontend
{
public:
    virtual void draw(Canvas *canvas) = 0;
    virtual void handleButtonPress(int x_global, int y_global, int button) {}
    virtual void handleButtonRelease(int button) {}
    virtual void handleMouseMove(int x, int y) {}
};

#endif
