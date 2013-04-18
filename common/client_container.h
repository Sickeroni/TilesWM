#ifndef __CLIENT_CONTAINER_H__
#define __CLIENT_CONTAINER_H__

#include "container.h"


class Canvas;


class ClientContainer : public Container
{
public:
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
    ClientContainer(ContainerContainer *parent);

    virtual void redraw() = 0;

    void draw(Canvas *canvas);
//     ClientContainer *splitContainer(Container *container, bool prepend_new_silbling);
//     ClientContainer *createSilblingFor(Container *container, bool prepend_new_silbling);
//     ClientContainer *getOrCreateSilblingFor(Container *container, bool get_prev);
    void moveClientToOther(Client *client, Direction dir);
    void clear();

private:
    enum Mode { TABBED, STACKED };

    static int _frame_width;
    static int _titlebar_height;

    int numMappedClients();
    void layoutTabbed();
    void layoutStacked();

    List<Client> _clients;
    Mode _mode;
};

#endif // __CLIENT_CONTAINER_H__
