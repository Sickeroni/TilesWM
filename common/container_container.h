#ifndef __CONTAINER_CONTAINER_H__
#define __CONTAINER_CONTAINER_H__

#include "container.h"


class ClientContainer;
class Canvas;
class ContainerContainerLayout;


class ContainerContainer : public Container
{
public:
    virtual ~ContainerContainer();

    virtual Container *child(int index) = 0;
    virtual int activeChildIndex() = 0;
    virtual void setActiveChild(int index) = 0;
    virtual int indexOfChild(const Container *child) = 0;
    // ret: index of added child
    virtual int addChild(Container *child) = 0;
    // inserts child at insert_pos
    virtual void insertChild(Container *child, int insert_pos) = 0;
    // de-parents and returns the child at index
    virtual Container *replaceChild(int index, Container *new_child) = 0;
    virtual Container *removeChild(int index) = 0;
    virtual int maxTextHeight() const = 0;
    virtual void redraw() = 0;

    virtual ContainerContainer *toContainerContainer() final { return this; }
    virtual ContainerLayout *getLayout() final;
    virtual ClientContainer *activeClientContainer() final;
    virtual void redrawAll() final;
    virtual void handleMaximizedChanged() final;
    virtual void handleActiveChanged() final;

//     void setDirty(bool set);

    void handleSizeHintsChanged(Container *child);

    Container *activeChild() {
        return activeChildIndex() >= 0 ? child(activeChildIndex()) : 0;
    }

protected:
    ContainerContainer();

private:
    void updateDirtyStatus();

//     bool _dirty; // is this container unused or are there unused child containers ?

    ContainerContainerLayout *_layout;
};


#endif // __CONTAINER_CONTAINER_H__
