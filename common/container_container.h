#ifndef __CONTAINER_CONTAINER_H__
#define __CONTAINER_CONTAINER_H__

#include "container.h"


class ClientContainer;
class Canvas;


class ContainerContainer : public Container
{
public:
    virtual ~ContainerContainer();

    virtual void redraw() = 0;
    virtual int numElements() = 0;
    virtual Container *activeChild() = 0;
    virtual Container *child(int index) = 0;
    virtual bool isEmpty() = 0;


    virtual ClientContainer *activeClientContainer();
//     virtual void setFocus();
//     virtual void addClient(Client *c);
    virtual void layout();
//     virtual void layoutClients();
    virtual void redrawAll();
//     virtual void deleteEmptyChildren();
    virtual void handleMaximizedChanged();
    virtual void handleActiveChanged();

//     void focusPrevChild();
//     void focusNextChild();
    void setDirty(bool set);
    void draw(Canvas *canvas);
//     ClientContainer *addNewClientContainer(bool prepend);
//     ClientContainer *splitChild(Container *child, bool prepend_new_sibling);
    
    void handleSizeHintsChanged(Container *child);
//     void incAvailableSpacePortion(Container *child, int pixels);
//     void decAvailableSpacePortion(Container *child, int pixels);

protected:
//     void clear() = 0;
//     virtual ClientContainer *createClientContainer() = 0;
//     virtual ContainerContainer *createContainerContainer() = 0;
    virtual int minWidthInt();
    virtual int maxWidthInt();
    virtual int minHeightInt();
    virtual int maxHeightInt();
    
    

    ContainerContainer(ContainerContainer *parent);

//     void appendChild(Container *container);
//     void prependChild(Container *container);
//     void replaceChild(Container *old_container, Container *new_container);
//     int hierarchyDepth();

private:
    void updateDirtyStatus();
    void deleteChild(Container *child);
//     int calcAvailableSpace();

//     List<Container> _children;
//     Container *_active_child;
    bool _dirty; // is this container unused or are there unused child containers ?
    double _reserved_space;

    static const int _max_hierarchy_depth = 1;
};


#endif // __CONTAINER_CONTAINER_H__
