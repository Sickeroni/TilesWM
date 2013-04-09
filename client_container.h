#ifndef __CLIENT_CONTAINER_H__
#define __CLIENT_CONTAINER_H__

#include "container.h"

class ClientContainer : public Container
{
public:
    ClientContainer(Container *parent, int x, int y, int w, int h);

    virtual void addClient(Client *c);
    virtual void layout();

    void removeClient(Client *c);
    ClientContainer *west()
    ClientContainer *getWest();

private:
    std::list<Client*> _clients;
};

#endif // __CLIENT_CONTAINER_H__
