#ifndef __CLIENT_CONTAINER_H__
#define __CLIENT_CONTAINER_H__

#include "container.h"


class Canvas;


class ClientContainer : public Container
{
public:
    virtual ~ClientContainer();

    virtual void redraw() = 0;

    virtual int numElements() { return _clients.count(); }
    virtual ClientContainer *activeClientContainer() { return this; }
    virtual void addClient(Client *c);
    virtual void layout();
//     virtual void layoutClients;
    virtual bool isEmpty() { return _clients.isEmpty(); }
    virtual void redrawAll();
    virtual int minimumWidth();
    virtual int minimumHeight();


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

    void moveClient(Direction where) {
        if (activeClient())
            moveClientToOther(activeClient(), where);
    }

protected:
    ClientContainer(ContainerContainer *parent);

    virtual int maxTextHeight() = 0;

    void draw(Canvas *canvas);
//     ClientContainer *splitContainer(Container *container, bool prepend_new_silbling);
    ClientContainer *createSilblingFor(Container *container, bool prepend_new_silbling);
    ClientContainer *getOrCreateSilblingFor(Container *container, bool get_prev);
    ClientContainer *getSilbling(bool get_prev);
    void moveClientToOther(Client *client, Direction dir);
    void clear();
    void getTabbbarRect(Rect &rect);
    void getClientRect(Rect &rect);
    int calcTabbarHeight();

    List<Client> _clients;

private:
    enum Mode { TABBED, STACKED };

    static const int _vertical_tabbar_width = 200;
    static const int _frame_width = 10;
    static const int _tab_inner_margin = 5;

    int numMappedClients();
    void getStackCellSize(int num_cells, int &w, int &h);
    void layoutTabbed();
    void layoutStacked(Client *about_to_be_mapped);
    void drawTabs(Canvas *canvas);
    void drawVerticalTabs(Canvas *canvas);
    void drawStacked(Canvas *canvas);
    void removeClientInt(Client *c, bool moving_to_new_container);
    void unfocusActiveClient();
    void drawTab(Client *client, const Rect &rect, Canvas *canvas);

    Mode _mode;
    Client *_active_client;
};

#endif // __CLIENT_CONTAINER_H__
