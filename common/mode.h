#ifndef __MODE_H__
#define __MODE_H__

#include <string>

class Workspace;
class ShortcutSet;
class Client;
class ContainerContainer;

class Mode
{
public:
    Mode(std::string name);
    virtual ~Mode();

    virtual void activate(Workspace *workspace) = 0; //FIXME workspace -> ContainerContainer ?
    virtual void initShortcuts() = 0;
    virtual void tileClient(Client *client, ContainerContainer *root_container) = 0;

    ShortcutSet *shortcuts() { return _shortcuts; }

private:
    ShortcutSet *_shortcuts;
};

#endif
