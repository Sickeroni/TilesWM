#ifndef __CLIENT_CONTAINER_H__
#define __CLIENT_CONTAINER_H__

#include "container.h"

#include "metrics.h"
#include "common.h"

#include <vector>


class Canvas;
class ClientContainerLayout;


class ClientContainer : public Container
{
public:
    virtual ~ClientContainer();

    virtual Client *child(size_t index) = 0;
    virtual int indexOfChild(const Client *child) = 0;
    virtual int activeChildIndex() = 0;
    // doesn't set focus to the client !
    virtual void setActiveChild(int index) = 0;
    // return: index of added client
    virtual int addChild(Client *client) = 0;
    virtual void removeChild(Client *client) = 0;
    // deparents all children and puts them into <clients>
    virtual void removeChildren(std::vector<Client*> &clients) = 0;
    virtual int maxTextHeight() = 0;
    virtual void redraw() = 0;

    virtual void redrawAll() final;
    virtual ContainerLayout *getLayout() final;
    virtual ClientContainer *activeClientContainer() final { return this; }

    Client *activeClient() {
        return activeChildIndex() >= 0 ? child(activeChildIndex()) : 0;
    }

    void handleMouseClick(int global_x, int global_y);
    void handleClientUnmap(Client *client);
    void handleClientFocusChange(Client *client);
    void handleClientSizeHintChanged(Client *client);

protected:
    ClientContainer();

private:
    ClientContainerLayout *_layout;
};

#endif // __CLIENT_CONTAINER_H__
