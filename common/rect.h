#ifndef __RECT_H__
#define __RECT_H__

struct Rect
{
    Rect() : x(0), y(0), w(0), h(0) {}
    Rect(const Rect &other);
    Rect& operator=(const Rect &other);

    void set(int x, int y, int w, int h) {
        this->x = x; this->y = y; this->w = w; this->h = h;
    }
//     void setPos(int x, int y) {
//         this->x = x; this->h = h;
//     }
    void setSize(int w, int h) {
        this->w = w; this->h = h;
    }

    int x, y, w, h;
};


#endif // __RECT_H__
