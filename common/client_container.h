#ifndef __CLIENT_CONTAINER_H__
#define __CLIENT_CONTAINER_H__

#include "container.h"


class ClientContainer : public Container
{
public:
    ClientContainer();
    virtual ~ClientContainer();

    virtual ClientContainer *activeClientContainer() {
        return this;
    }
    virtual void addClient(Client *c);
    virtual void layout();
//     virtual void layoutClients;
    virtual bool isEmpty() { return _clients.isEmpty(); }

    void removeClient(Client *c);

protected:
    ClientContainer *splitContainer(Container *container, bool prepend_new_silbling);
    ClientContainer *createSilblingFor(Container *container, bool prepend_new_silbling);
    ClientContainer *getOrCreateSilblingFor(Container *container, bool get_prev);
    void moveClientToOther(Client *client, Direction dir);

private:
    static int _frame_width;
    static int _titlebar_height;

    List<Client> _clients;

    int numMappedClients();
};

#endif // __CLIENT_CONTAINER_H__
