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
    virtual void layout();
//     virtual void layoutClients;
    virtual bool isEmpty() { return _clients.isEmpty(); }
    virtual void redrawAll();

    Client *activeClient();
    void setActiveClient(Client *client);

    void removeClient(Client *c) {
        removeClientInt(c, false);
    }

    void getClientSize(int &w, int &h);

    void focusPrevClient();
    void focusNextClient();

    void handleMouseClick(int global_x, int global_y);
    void handleClientMap(Client *client);
    void handleClientUnmap(Client *client);
    void handleClientAboutToBeMapped(Client *client);
    void handleClientFocusChange(Client *client);
    void handleClientSizeHintChanged(Client *client);

//     void createSibling(Direction where);
    void focusSibling(Direction where);

    void moveClient(Direction where) {
        if (activeClient())
            moveClientToOther(activeClient(), where);
    }



//     void incCustomSize()
//     void decCustomSize();
//     void setCustomSizeActive(bool active);

protected:
    virtual int minWidthInt();
    virtual int maxWidthInt();
    virtual int minHeightInt();
    virtual int maxHeightInt();

    ClientContainer(ContainerContainer *parent);

    virtual int maxTextHeight() = 0;

    void draw(Canvas *canvas);
//     ClientContainer *splitContainer(Container *container, bool prepend_new_sibling);
    ClientContainer *createSiblingFor(Container *container, bool prepend_new_sibling);
    ClientContainer *getOrCreateSiblingFor(Container *container, bool get_prev);
    ClientContainer *getSibling(bool get_prev);
    void moveClientToOther(Client *client, Direction dir);
    void clear();
    void getTabbbarRect(Rect &rect);
    void getClientRect(Rect &rect);
    int calcTabbarHeight();

    List<Client> _clients;

private:
    enum Mode { TABBED, STACKED };

    static const int _vertical_tabbar_width = 70;
    static const int _frame_width = Metrics::CLIENT_CONTAINER_FRAME_MARGIN;
    static const int _tab_inner_margin = 5;
    static const int _tab_gap = 2;
    static const int _status_bar_width = 30;

    int numMappedClients();
    void getTabSize(int &tab_width, int &tab_height);
    void getStackCellSize(int num_cells, int &w, int &h);
    void layoutTabbed();
    void layoutStacked(Client *about_to_be_mapped);
    void drawTabbar(Canvas *canvas);
    void drawVerticalTabs(Canvas *canvas);
    void drawTab(Client *client, const Rect &rect, bool minimized, bool vertical, Canvas *canvas);
    void drawStacked(Canvas *canvas);
    void removeClientInt(Client *c, bool moving_to_new_container);
    void unfocusActiveClient();

    // this value is addet to the client-specified minimum size
    // configurable by user
//     int _extra_space;
//     bool _custom_size_active;
    Mode _mode;
    Client *_active_client;
};

#endif // __CLIENT_CONTAINER_H__
