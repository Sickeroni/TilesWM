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

    virtual void redraw() = 0;
    virtual int numElements() = 0;
    virtual Container *child(int index) = 0;
    virtual int activeChildIndex() = 0;
    virtual void setActiveChild(int index) = 0;
    virtual int indexOfChild(const Container *child) = 0;
    virtual bool isEmpty() = 0;


    virtual ContainerLayout *getLayout();
    virtual ClientContainer *activeClientContainer();
    virtual void redrawAll();
//     virtual void deleteEmptyChildren();
    virtual void handleMaximizedChanged();
    virtual void handleActiveChanged();

    void setDirty(bool set);

    void handleSizeHintsChanged(Container *child);
//     void incAvailableSpacePortion(Container *child, int pixels);
//     void decAvailableSpacePortion(Container *child, int pixels);

    Container *activeChild() {
        return activeChildIndex() > -1 ? child(activeChildIndex()) : 0;
    }

protected:
    ContainerContainer();

private:
    void updateDirtyStatus();
    void deleteChild(Container *child);
//     int calcAvailableSpace();

//     bool _dirty; // is this container unused or are there unused child containers ?
//     double _reserved_space;

    ContainerContainerLayout *_layout;

    static const int _max_hierarchy_depth = 1;
};


#endif // __CONTAINER_CONTAINER_H__
