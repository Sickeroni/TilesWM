#ifndef __CLIENT_CONTAINER_LAYOUT_H__
#define __CLIENT_CONTAINER_LAYOUT_H__

#include "container_layout.h"

class ClientContainer;

class ClientContainerLayout final : public ContainerLayout
{
public:
    ClientContainerLayout(ClientContainer *container);

    virtual int minWidth() override;
    virtual int maxWidth() override;
    virtual int minHeight() override;
    virtual int maxHeight() override;
    virtual void layoutContents() override;

private:
    ClientContainer *_container;
};

#endif // __CLIENT_CONTAINER_LAYOUT_H__
