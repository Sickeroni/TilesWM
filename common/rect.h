#ifndef __RECT_H__
#define __RECT_H__

struct Rect
{
    Rect() : x(0), y(0), w(0), h(0) {}

    void set(int x, int y, int w, int h);
    void setPos(int x, int y);
    void setSize(int w, int h);

    int x, y, w, h;
};


#endif // __RECT_H__
