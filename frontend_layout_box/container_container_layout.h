#ifndef __CONTAINER_CONTAINER_LAYOUT_H__
#define __CONTAINER_CONTAINER_LAYOUT_H__

#include "container_layout.h"

class ContainerContainer;

class ContainerContainerLayout final : public ContainerLayout
{
public:
    ContainerContainerLayout(ContainerContainer *container);

    virtual int minWidth() override;
    virtual int maxWidth() override;
    virtual int minHeight() override;
    virtual int maxHeight() override;
    virtual void layoutContents() override;

private:
    ContainerContainer *_container;
};

#endif // __CONTAINER_CONTAINER_LAYOUT_H__
