#include "container_util.h"

#include "container_container.h"
#include "container_layout.h"
#include "client_container.h"
#include "application.h"

namespace ContainerUtil
{


int hierarchyDepth(Container *container)
{
    if (container->parentContainer())
        return hierarchyDepth(container->parentContainer()) + 1;
    else
        return 0;
}

ClientContainer *createSibling(Container *container, bool prepend)
{
    ClientContainer *new_sibling = 0;
    ContainerContainer *parent = container->parentContainer();

    if (parent) {
        new_sibling = new ClientContainer();
        int insert_pos = parent->indexOfChild(container);
        if (!prepend)
            insert_pos++;
        parent->insertChild(new_sibling, insert_pos);
        parent->setActiveChild(insert_pos);
    }

    return new_sibling;
}

ClientContainer *getSibling(Container *container, bool get_prev, bool create_new_if_not_existing)
{
    ContainerContainer *parent = container->parentContainer();
    if (!parent)
        return 0;

    int index = parent->indexOfChild(container);

    if (get_prev && (index > 0))
        return parent->child(index - 1)->activeClientContainer();
    else if (!get_prev && ((index + 1) < parent->numElements()))
        return parent->child(index + 1)->activeClientContainer();
    else if (create_new_if_not_existing)
        return createSibling(container, get_prev);
    else
        return 0;
}

ClientContainer *splitContainer(ClientContainer *container, bool prepend)
{
    static const int max_hierarchy_depth = 1;

    ContainerContainer *parent = container->parentContainer();

    if (!parent)
        return 0;

    if (hierarchyDepth(parent) >= max_hierarchy_depth)
        return 0;

    ContainerContainer *new_parent = new ContainerContainer();

    parent->replaceChild(parent->indexOfChild(container), new_parent); // de-parents container

    ClientContainer *new_sibling = new ClientContainer();

    // add this + new child container to new parent
    if (prepend) {
        new_parent->addChild(new_sibling);
        new_parent->addChild(container);
    } else {
        new_parent->addChild(container);
        new_parent->addChild(new_sibling);
    }

    new_parent->setActiveChild(new_parent->indexOfChild(new_sibling));

    return new_sibling;
}

void emptyContainer(ContainerContainer *container, std::vector<Client*> &clients)
{
    while(!container->isEmpty()) {
        if (container->child(0)->isContainerContainer())
            emptyContainer(container->child(0)->toContainerContainer(), clients);
        else
            container->child(0)->toClientContainer()->removeChildren(clients);
        delete container->removeChild(0);
    }
}

void deleteEmptyChildren(ContainerContainer *container)
{
    for (int i = 0; i < container->numElements(); ) {
        Container *child = container->child(i);

        if (child->isContainerContainer()) {
            // recurse
            deleteEmptyChildren(child->toContainerContainer());

            // dissolve child containers with only one child
            if (child->numElements() == 1) {
                Container *c = child->toContainerContainer()->removeChild(0);
                assert(c->isClientContainer());

                container->replaceChild(i, c);
                delete child;
                child = c;
            }
        }

        if (child->isEmpty()) {
            container->removeChild(i);
            delete child;
        } else
            i++;
    }

    container->getLayout()->layoutContents();
}


} // namespace ContainerUtil
