#ifndef __CONTAINER_CONTAINER_H__
#define __CONTAINER_CONTAINER_H__

#include "container.h"


class ClientContainer;
class Canvas;


class ContainerContainer : public Container
{
public:
    virtual ~ContainerContainer();


    virtual ClientContainer *activeClientContainer();

    Container *activeChild();

    virtual void redraw() = 0;
    virtual void addClient(Client *c);
    virtual void layout();
//     virtual void layoutClients();
    virtual bool isEmpty() { return _children.isEmpty(); }


    void deleteEmptyChildren();
    void setDirty(bool set);
    void draw(Canvas *canvas);

    void appendNewClientContainer();

protected:
    virtual ClientContainer *createClientContainer() = 0;

    ContainerContainer(ContainerContainer *parent);

    void appendChild(Container *container);
//     void prependChild(Container *container);
//     void replaceChild(Container *old_container, Container *new_container);
    void clear();

private:
    void updateDirtyStatus();
    void deleteChild(Container *child);
    void getClientRect(Rect &rect);

    List<Container> _children;
    bool _dirty; // is this container unused or are there unused child containers ?;

    static const int frame_width = 10;
    static const int title_height = 10;
    static const int child_frame_width = 10;
};


#endif // __CONTAINER_CONTAINER_H__
