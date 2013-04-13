#ifndef __CONTAINER_CONTAINER_H__
#define __CONTAINER_CONTAINER_H__

#include "container.h"


class ClientContainer;


class ContainerContainer : public Container
{
public:
    ContainerContainer();

    virtual ClientContainer *activeClientContainer();
    virtual void addClient(Client *c);
    virtual void layout();
//     virtual void layoutClients();
    virtual bool isEmpty() { return _children.isEmpty(); }

    void prependChild(Container *container);
    void appendChild(Container *container);
    void replaceChild(Container *old_container, Container *new_container);

    void deleteEmptyChildren();

    void setDirty(bool set);

private:
    void updateDirtyStatus();
    void deleteChild(Container *child);

    List<Container> _children;
    bool _dirty; // is this container unused or are there unused child containers ?;
};


#endif // __CONTAINER_CONTAINER_H__
