#ifndef __CONTAINER_LAYOUT_H__
#define __CONTAINER_LAYOUT_H__

class ContainerLayout
{
public:
    virtual int minWidth() = 0;
    virtual int maxWidth() = 0;
    virtual int minHeight() = 0;
    virtual int maxHeight() = 0;
}

#endif // __CONTAINER_LAYOUT_H__
