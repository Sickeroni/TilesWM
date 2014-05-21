#ifndef __CLIENT_CONTAINER_H__
#define __CLIENT_CONTAINER_H__

#include "container.h"

#include "metrics.h"


class Canvas;


class ClientContainer : public Container
{
public:
    virtual ~ClientContainer();

    virtual void redraw() = 0;

    virtual int numElements() { return _clients.count(); }
    virtual ClientContainer *activeClientContainer() { return this; }
    virtual void addClient(Client *c);
//     virtual void layout();
//     virtual void layoutClients;
    virtual bool isEmpty() { return _clients.isEmpty(); }
    virtual void redrawAll();

    virtual Client *child(int index) = 0;
    // doesn't set focus to the client !
    virtual void setActiveChild(int index) = 0;
    virtual int maxTextHeight() = 0;

    Client *activeClient();
    void setActiveClient(Client *client);

    void removeClient(Client *c) {
        abort();
//         removeClientInt(c, false);
    }

    void focusPrevClient();
    void focusNextClient();

    void handleMouseClick(int global_x, int global_y);
    void handleClientMap(Client *client);
    void handleClientUnmap(Client *client);
    void handleClientAboutToBeMapped(Client *client);
    void handleClientFocusChange(Client *client);
    void handleClientSizeHintChanged(Client *client);

//     void createSibling(Direction where);
//     void focusSibling(Direction where);

//     void moveClient(Direction where) {
//         if (activeClient())
//             moveClientToOther(activeClient(), where);
//     }



//     void incCustomSize()
//     void decCustomSize();
//     void setCustomSizeActive(bool active);

protected:
    ClientContainer(ContainerContainer *parent);

    void draw(Canvas *canvas);
//     ClientContainer *splitContainer(Container *container, bool prepend_new_sibling);
//     ClientContainer *createSiblingFor(Container *container, bool prepend_new_sibling);
//     ClientContainer *getOrCreateSiblingFor(Container *container, bool get_prev);
//     ClientContainer *getSibling(bool get_prev);
//     void moveClientToOther(Client *client, Direction dir);
    void clear();

#if 0    
    void getTabbbarRect(Rect &rect);
    void getClientRect(Rect &rect);
    int calcTabbarHeight();
#endif

    List<Client> _clients;

private:
    int numMappedClients();
    void getTabSize(int &tab_width, int &tab_height);
    void drawTabbar(Canvas *canvas);
    void drawVerticalTabs(Canvas *canvas);
    void drawTab(Client *client, const Rect &rect, bool minimized, bool vertical, Canvas *canvas);
    void removeClientInt(Client *c, bool moving_to_new_container);
    void unfocusActiveClient();

    // this value is addet to the client-specified minimum size
    // configurable by user
//     int _extra_space;
//     bool _custom_size_active;
    Client *_active_client;
};

#endif // __CLIENT_CONTAINER_H__
