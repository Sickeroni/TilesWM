#ifndef __CLIENT_CONTAINER_H__
#define __CLIENT_CONTAINER_H__

#include "container.h"


class Canvas;


class ClientContainer : public Container
{
public:
    virtual ~ClientContainer();

    virtual void redraw() = 0;

    virtual ClientContainer *activeClientContainer() {
        return this;
    }
    virtual void addClient(Client *c);
    virtual void layout();
//     virtual void layoutClients;
    virtual bool isEmpty() { return _clients.isEmpty(); }
    virtual void redrawAll();

    bool hasFocus() {
        //FIXME HACK
        return true;
    }

    Client *activeClient();
    void setActiveClient(Client *client);

    void removeClient(Client *c) {
        removeClientInt(c, false);
    }

    void getClientSize(int &w, int &h);

    void focusPrevClient();
    void focusNextClient();

    void handleClientMap(Client *client);
    void handleClientUnmap(Client *client);
    void handleClientAboutToBeMapped(Client *client);
    void handleClientFocusChange(Client *client);

    void createSilbling(Direction where);
    void focusSilbling(Direction where);

protected:
    ClientContainer(ContainerContainer *parent);

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
    void getStackCellSize(int num_cells, int &w, int &h);
    void layoutTabbed();
    void layoutStacked(Client *about_to_be_mapped);
    void drawTabbed(Canvas *canvas);
    void drawStacked(Canvas *canvas);
    void removeClientInt(Client *c, bool moving_to_new_container);
    void unfocusActiveClient();

    List<Client> _clients;
    Mode _mode;
    Client *_active_client;
};

#endif // __CLIENT_CONTAINER_H__
