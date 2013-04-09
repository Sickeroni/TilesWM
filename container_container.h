#ifndef __CONTAINER_CONTAINER_H__
#define __CONTAINER_CONTAINER_H__

#include "container.h"

#include <list>

class ClientContainer;

typedef std::list<Container*> ContainerList;

class ContainerContainer : public Container
{
public:
    ContainerContainer(ContainerContainer *parent, int x, int y, int width, int height);

    virtual void addClient(Client *c);
    virtual void layout();
    virtual ClientContainer *currentClientContainer();

    void addContainer(Container *container);
    Container *prev(Container *from);
    Container *next(Container *from);
    ClientContainer *findSilblingOf(Container *which, Direction dir);
    ClientContainer *getOrCreateSilblingOf(Container *which, Direction dir);

private:
    ContainerList::iterator find(Container *container);

    ContainerList _children;
};

#endif // __CONTAINER_CONTAINER_H__
