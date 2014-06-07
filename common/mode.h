#ifndef __MODE_H__
#define __MODE_H__

#include "action_set.h"

class Workspace;
class Client;
class ContainerContainer;

class Mode : public ActionSet
{
public:
    Mode(std::string name) : ActionSet(name) {}

    virtual void activate(Workspace *workspace) = 0; //FIXME workspace -> ContainerContainer ?
    virtual void tileClient(Client *client, ContainerContainer *root_container) = 0;
};

#endif
