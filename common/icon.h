#ifndef __ICON_H__
#define __ICON_H__

class Icon
{
public:
    Icon(int width, int height) : _width(width), _height(height) {}
    Icon(const Icon &other);
    Icon &operator=(const Icon &other);
    virtual ~Icon() {}

    // create a scaled copy
    virtual Icon *scale(int width, int height) { return 0; }

    int width() { return _width; }
    int height() { return _height; }

protected:
    int _width, _height;
};

#endif // __ICON_H__
