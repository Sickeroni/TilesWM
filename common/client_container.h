#ifndef __CLIENT_CONTAINER_H__
#define __CLIENT_CONTAINER_H__

#include "container.h"

#include "metrics.h"
#include "common.h"


class Canvas;
class ClientContainerLayout;


class ClientContainer : public Container
{
public:
    virtual ~ClientContainer();

    virtual int numElements() = 0;
    virtual bool isEmpty() = 0;
    virtual Client *child(int index) = 0;
    virtual int indexOfChild(Client *child) = 0;
    virtual int activeChildIndex() = 0;
    // doesn't set focus to the client !
    virtual void setActiveChild(int index) = 0;
    virtual int maxTextHeight() = 0;
    virtual void redraw() = 0;

    virtual void redrawAll();
    virtual ContainerLayout *getLayout();
    virtual ClientContainer *activeClientContainer() { return this; }

    Client *activeClient() {
        return activeChildIndex() > -1 ? child(activeChildIndex()) : 0;
    }

    void removeClient(Client *c) {
        abort();
    }

    void handleMouseClick(int global_x, int global_y);
    void handleClientMap(Client *client);
    void handleClientUnmap(Client *client);
    void handleClientAboutToBeMapped(Client *client);
    void handleClientFocusChange(Client *client);
    void handleClientSizeHintChanged(Client *client);

//     void incCustomSize()
//     void decCustomSize();
//     void setCustomSizeActive(bool active);

protected:
    ClientContainer(ContainerContainer *parent);

private:
//     void removeClientInt(Client *c, bool moving_to_new_container);
//     void unfocusActiveClient();

    // this value is addet to the client-specified minimum size
    // configurable by user
//     int _extra_space;
//     bool _custom_size_active;
    ClientContainerLayout *_layout;
};

#endif // __CLIENT_CONTAINER_H__
