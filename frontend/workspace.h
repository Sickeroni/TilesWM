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
class AbstractKeySequence;

class Workspace final : public Widget
{
public:
    Workspace();
    ~Workspace();

    virtual void setRect(const Rect &rect) override;
    virtual void draw(Canvas *canvas) override;

    Monitor *monitor() { return _monitor; }

    bool makeActive();
    void layoutContents();

    bool isActive() {
        return monitor() && monitor()->isActive();
    }
    void setMonitor(Monitor *monitor) {
        _monitor = monitor;
        if (!_monitor)
            setHasFocus(false);
        setMapped(monitor != 0);
        refreshStatusText();
    }
    Mode *mode();
    size_t modeIndex() { return _mode; }
    void setMode(size_t index);

    void addClient(Client *client);
    void removeClient(Client *client);
    ClientWrapper *activeClient();

    void redrawAll();
    void refreshStatusText();

    Icon *background() { return _background_scaled; }

    void setHasFocus(bool has_focus);

    void makeClientActive(const Client *client);
    bool handleKeySequence(const AbstractKeySequence *sequence);

private:
    class Layout;

    Monitor *_monitor = 0;
    size_t _mode = 0;
    bool _has_focus = false;
    Icon *_background = 0, *_background_scaled = 0;
    std::list<Client*> _clients;
    std::vector<Layout*> _layouts;
    Layout *_active_layout = 0;
};

#endif // __WORKSPACE_H__
