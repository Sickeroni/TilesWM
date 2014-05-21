#ifndef __CONTAINER_CONTAINER_LAYOUT_H__
#define __CONTAINER_CONTAINER_LAYOUT_H__

#include "container_layout.h"

class ContainerContainer;

class ContainerContainerLayout : public ContainerLayout
{
public:
    ContainerContainerLayout(ContainerContainer *container);

    virtual int minWidth();
    virtual int maxWidth();
    virtual int minHeight();
    virtual int maxHeight();
    virtual void layoutContents();

private:
    ContainerContainer *_container;
};

#endif // __CONTAINER_CONTAINER_LAYOUT_H__
