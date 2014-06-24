#ifndef __WORKSPACE_H__
#define __WORKSPACE_H__

#include "monitor.h"
#include "rect.h"
#include "container.h"

#include <cstddef>

class ClientContainer;
class Client;
class Mode;
class WindowManager;

class Workspace
{
public:
    Workspace();
    ~Workspace();

    Monitor *monitor() { return _monitor; }
    bool maximized() { return _maximized; } //FIXME -> isMaximized()
    void setMaximized(bool enable);
    bool makeActive();
    bool isMapped();
    Client *activeClient(); // active client in the floating layer
    void addClient(Client *client);
    void removeClient(Client *client);
    void layoutContents();
    void setRect(Rect rect);
    const Rect &rect() { return _rect; }

    bool isActive() {
        return monitor() && monitor()->isActive();
    }
    void setMonitor(Monitor *monitor) {
        _monitor = monitor;
    }
    Mode *mode();
    size_t modeIndex() { return _mode; }
    void setMode(size_t index);
    Container::Orientation orientation() { return _orientation; }
    void rotateOrientation();

    WindowManager *windowManager() { return _window_manager; }

private:
    Rect _rect;
    Monitor *_monitor = 0;
    //ContainerContainer *_dock
    bool _maximized = false;
    size_t _mode = 0;
    WindowManager *_window_manager = 0;
    Container::Orientation _orientation = Container::HORIZONTAL;
};

#endif // __WORKSPACE_H__
