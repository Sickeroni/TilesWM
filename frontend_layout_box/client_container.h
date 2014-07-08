#ifndef __CLIENT_CONTAINER_H__
#define __CLIENT_CONTAINER_H__

#include "container.h"

#include "metrics.h"
#include "common.h"

#include <vector>


class Canvas;
class ClientContainerLayout;


class ClientContainer final : public Container
{
public:
    ClientContainer();
    virtual ~ClientContainer();

    virtual void draw(Canvas *canvas) override;

    virtual int numElements() const override { return _children.size(); }
    virtual ClientContainer *activeClientContainer() override { return this; }
    virtual bool isEmpty() const override { return _children.empty(); }
    virtual void redrawAll() override {
        redraw();
    }
    virtual ContainerLayout *getLayout() override;
    virtual bool isMinimized() const override { return _is_minimized; }

    Client *child(size_t index) { return _children[index]; }
    int indexOfChild(const Client *child);
    int activeChildIndex() { return _active_child_index; }
    // doesn't set focus to the client !
    void setActiveChild(int index);
    // return: index of added client
    int addChild(Client *client);
    void removeChild(Client *client);
    // deparents all children and puts them into <clients>
    void removeChildren(std::vector<Client*> &clients);
    void setMinimized(bool minimized);
//     void clear();

    Client *activeClient() {
        return activeChildIndex() != INVALID_INDEX ? child(activeChildIndex()) : 0;
    }

    void handleClientFocusChange(Client *client);
    void handleClientSizeHintChanged(Client *client);

private:
    ClientContainerLayout *_layout = 0;
    bool _is_minimized = false;
    std::vector<Client*> _children;
    int _active_child_index = INVALID_INDEX;
};

#endif // __CLIENT_CONTAINER_H__
