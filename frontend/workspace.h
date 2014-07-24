#ifndef __WORKSPACE_H__
#define __WORKSPACE_H__

#include "monitor.h"
#include "rect.h"
#include "widget.h"
#include "common.h"

#include <list>
#include <vector>

class Client;
class ClientWrapper;
class Mode;
class WindowManager;
class ChildWidget;
class Icon;

class Workspace final : public Widget
{
public:
    Workspace();
    ~Workspace();

    virtual void setRect(const Rect &rect) override;

    Monitor *monitor() { return _monitor; }

    bool makeActive();
    void layoutContents();

    bool isActive() {
        return monitor() && monitor()->isActive();
    }
    void setMonitor(Monitor *monitor) {
        _monitor = monitor;
        setMapped(monitor != 0);
        refreshStatusText();
    }
    Mode *mode();
    size_t modeIndex() { return _mode; }
    void setMode(size_t index);

    WindowManager *windowManager() { return _window_manager; }

    void addChild(ChildWidget *child);
    void removeChild(ChildWidget *child);

    void addClient(Client *client);
    void removeClient(Client *client);
    ClientWrapper *activeClient();
    void makeClientActive(Client *client);

    void redrawAll();
    void refreshStatusText();

    Icon *background() { return _background_scaled; }

private:
    Monitor *_monitor = 0;
    //ContainerContainer *_dock
    size_t _mode = 0;
    WindowManager *_window_manager = 0;
    Icon *_background = 0, *_background_scaled = 0;
    std::list<Client*> _clients;
    std::vector<WindowManager*> _window_managers;
};

#endif // __WORKSPACE_H__
