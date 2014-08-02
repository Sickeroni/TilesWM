#ifndef __RECT_H__
#define __RECT_H__

struct Rect
{
    Rect() : x(0), y(0), w(0), h(0) {}
    Rect(const Rect &other) {
        set(other);
    }
    Rect(int x, int y, int w, int h) {
        set(x, y, w, h);
    }
    Rect& operator=(const Rect &other) {
        set(other);
        return *this;
    }

    bool isValid() {
        return (x >= 0 && y >= 0);
    }

    void set(const Rect &other) {
        this->x = other.x; this->y = other.y; this->w = other.w; this->h = other.h;
    }

    void set(int x, int y, int w, int h) {
        this->x = x; this->y = y; this->w = w; this->h = h;
    }
    void setPos(int x, int y) {
        this->x = x; this->y = y;
    }
    void setSize(int w, int h) {
        this->w = w; this->h = h;
    }

    bool isPointInside(int x, int y) {
        return x >= this->x && x < (this->x + this->w) &&
               y >= this->y && y < (this->y + this->h);
    }

    int x, y, w, h;
};


#endif // __RECT_H__
