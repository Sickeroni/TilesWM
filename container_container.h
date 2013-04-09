#ifndef __CONTAINER_CONTAINER_H__
#define __CONTAINER_CONTAINER_H__

#include "container.h"

class ContainerContainer : public Container
{
public:
    ContainerContainer(Container *parent, int x, int y, int width, int height);

    virtual void addClient(Client *c);
    virtual void layout();

    void addContainer(Container *container);
};

#endif // __CONTAINER_CONTAINER_H__
