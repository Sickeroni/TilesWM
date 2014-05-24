#ifndef __WORKSPACE_H__
#define __WORKSPACE_H__

#include "monitor.h"
#include "rect.h"

class ContainerContainer;
class ClientContainer;
class Client;

class Workspace
{
public:
    Workspace();
    ~Workspace();

    Monitor *monitor() { return _monitor; }
    ContainerContainer *rootContainer() { return _root_container; }
    bool maximized() { return _maximized; } //FIXME -> isMaximized()
    void setMaximized(bool enable);
    bool makeActive();
    bool isMapped();
    Client *activeClient(); // active client in the floating layer
    void layoutContents();
    void setRect(Rect rect);

    bool isActive() {
        return monitor() && monitor()->isActive();
    }
    void setMonitor(Monitor *monitor) {
        _monitor = monitor;
    }
    void globalToLocal(int &x, int &y);

private:
    Rect _rect;
    Monitor *_monitor;
    //ContainerContainer *_dock
    ContainerContainer *_root_container;
    bool _maximized;
};

#endif // __WORKSPACE_H__
