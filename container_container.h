#ifndef __CONTAINER_CONTAINER_H__
#define __CONTAINER_CONTAINER_H__

#include "container.h"

class ClientContainer;

class ContainerContainer : public Container
{
public:
    ContainerContainer(Container *parent, int x, int y, int width, int height);

    virtual void addClient(Client *c);
    virtual void layout();

    void addContainer(Container *container);
    ClientContainer *findSilblingOf(ClientContainer *which, Direction dir);
    ClientContainer *getOrCreateSilblingOf(ClientContainer *which, Direction dir);
};

#endif // __CONTAINER_CONTAINER_H__
