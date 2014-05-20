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
    virtual int numElements() { return _children.count(); }
    virtual ClientContainer *activeClientContainer();
    virtual void setFocus();
//     virtual void addClient(Client *c);
    virtual void layout();
//     virtual void layoutClients();
    virtual bool isEmpty() { return _children.isEmpty(); }
    virtual void redrawAll();
    virtual void deleteEmptyChildren();
    virtual void handleMaximizedChanged();
    virtual void handleActiveChanged();

    Container *activeChild() { return _active_child; }
    void focusPrevChild();
    void focusNextChild();
    void setDirty(bool set);
    void draw(Canvas *canvas);
//     ClientContainer *addNewClientContainer(bool prepend);
//     ClientContainer *splitChild(Container *child, bool prepend_new_sibling);
    void setActiveChild(Container *child);
    void handleSizeHintsChanged(Container *child);
//     void incAvailableSpacePortion(Container *child, int pixels);
//     void decAvailableSpacePortion(Container *child, int pixels);


protected:
//     virtual ClientContainer *createClientContainer() = 0;
//     virtual ContainerContainer *createContainerContainer() = 0;
    virtual int minWidthInt();
    virtual int maxWidthInt();
    virtual int minHeightInt();
    virtual int maxHeightInt();

    ContainerContainer(ContainerContainer *parent);

    void appendChild(Container *container);
    void prependChild(Container *container);
    void replaceChild(Container *old_container, Container *new_container);
    void clear();
    int hierarchyDepth();

private:
    void updateDirtyStatus();
    void deleteChild(Container *child);
    void getClientRect(Rect &rect);
    int calcAvailableSpace();

    List<Container> _children;
    Container *_active_child;
    bool _dirty; // is this container unused or are there unused child containers ?
    double _reserved_space;

    static const int _frame_width = 10;
    static const int _title_height = 10;
    static const int _child_frame_width = 10;

    static const int _max_hierarchy_depth = 1;
};


#endif // __CONTAINER_CONTAINER_H__
