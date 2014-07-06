#ifndef __WORKSPACE_H__
#define __WORKSPACE_H__

#include "monitor.h"
#include "rect.h"
#include "widget.h"
#include "common.h"

class Client;
class Mode;
class WindowManager;
class ChildWidget;

class Workspace final : public Widget
{
public:
    Workspace();
    ~Workspace();

    Monitor *monitor() { return _monitor; }

    bool makeActive();
    void layoutContents();

    bool isActive() {
        return monitor() && monitor()->isActive();
    }
    void setMonitor(Monitor *monitor) {
        _monitor = monitor;
        setMapped(monitor != 0);
    }
    Mode *mode();
    size_t modeIndex() { return _mode; }
    void setMode(size_t index);

    WindowManager *windowManager() { return _window_manager; }

    // active child in the floating layer
    ChildWidget *activeFlotatingChild() {
        return _active_floating_child;
    }
    // sets the active client in the floating layer
    void setActiveFloatingChild(ChildWidget *child);

    Client *activeFloatingClient();

    void addChild(ChildWidget *child);
    void removeChild(ChildWidget *child);

private:
    Monitor *_monitor = 0;
    //ContainerContainer *_dock
    size_t _mode = 0;
    WindowManager *_window_manager = 0;
    ChildWidget *_active_floating_child = 0;
};

#endif // __WORKSPACE_H__
