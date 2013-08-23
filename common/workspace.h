#ifndef __WORKSPACE_H__
#define __WORKSPACE_H__

class ContainerContainer;
class ClientContainer;

class Workspace
{
public:
    ContainerContainer *rootContainer() { return _root_container; }
    void setRootContainer(ContainerContainer *container);
    bool maximized() { return _maximized; }
    void setMaximized(bool enable);

    Workspace();

private:
    //ContainerContainer *_dock
    ContainerContainer *_root_container;
    bool _maximized;
};

#endif // __WORKSPACE_H__
