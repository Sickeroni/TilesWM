#ifndef __CLIENT_CONTAINER_H__
#define __CLIENT_CONTAINER_H__

#include "container.h"

#include "container_container.h"

class ClientContainer : public Container
{
public:
    ClientContainer(ContainerContainer *parent, int x, int y, int w, int h);

    virtual void addClient(Client *c);
    virtual void layout();
    virtual ClientContainer *currentClientContainer() {
        return this;
    }

    void removeClient(Client *c);

    ClientContainer *findSilbling(Direction dir) {
        return _parent ? 0 : _parent->findSilblingOf(this, dir);
    }
    ClientContainer *getOrCreateSilbling(Direction dir);

private:
    std::list<Client*> _clients;
};

#endif // __CLIENT_CONTAINER_H__
