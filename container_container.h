#ifndef __CONTAINER_CONTAINER_H__
#define __CONTAINER_CONTAINER_H__

#include "container.h"

#include <list>

class ClientContainer;

// typedef std::list<Container*> ContainerList;

class ContainerContainer : public Container
{
public:
    ContainerContainer();

    virtual ClientContainer *activeClientContainer();
    virtual void addClient(Client *c);
    virtual void layout();
//     virtual void layoutClients;

    void prependChild(Container *container);
    void appendChild(Container *container);
    void replaceChild(Container *old_container, Container *new_container);

    void deleteEmptyChildren();

private:
    void setDirty(bool set);
    void updateDirtyStatus();

    bool _dirty; // is this container unused or are there unused child containers ?;
};

#endif // __CONTAINER_CONTAINER_H__
