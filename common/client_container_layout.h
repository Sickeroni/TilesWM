#ifndef __CLIENT_CONTAINER_LAYOUT_H__
#define __CLIENT_CONTAINER_LAYOUT_H__

#include "container_layout.h"

class ClientContainer;

class ClientContainerLayout : public ContainerLayout
{
public:
    ClientContainerLayout(ClientContainer *container);

    virtual int minWidth();
    virtual int maxWidth();
    virtual int minHeight();
    virtual int maxHeight();
    virtual void layoutContents();

private:
    ClientContainer *_container;
};

#endif // __CLIENT_CONTAINER_LAYOUT_H__
