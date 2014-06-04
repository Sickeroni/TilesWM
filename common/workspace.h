#ifndef __WORKSPACE_H__
#define __WORKSPACE_H__

#include "monitor.h"
#include "rect.h"

#include <cstddef>

class ContainerContainer;
class ClientContainer;
class Client;
class Mode;

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
    Mode *mode();
    size_t modeIndex() { return _mode; }
    void setMode(size_t index);

private:
    Rect _rect;
    Monitor *_monitor = 0;
    //ContainerContainer *_dock
    ContainerContainer *_root_container = 0;
    bool _maximized = false;
    size_t _mode = 0;
};

#endif // __WORKSPACE_H__
