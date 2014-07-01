#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "common.h"

#include <vector>
#include <cstddef>
#include <string>

class Workspace;
class Client;
class Monitor;
class ShortcutSet;
class Mode;
class ActionSet;
class WidgetBackend;
class Widget;

class Application
{
public:
    enum Layer
    {
        LAYER_FLOATING,
        LAYER_TILED
    };

    ~Application();

    virtual Monitor *activeMonitor() = 0;

    virtual void setActiveMonitor(Monitor *monitor) = 0;
    virtual void requestQuit() = 0;
    virtual WidgetBackend *createWidgetBackend() = 0;
    virtual ShortcutSet *createShortcutSet(std::string name) = 0;
    virtual Workspace *createWorkspace() = 0; //FIXME make non-virtual

    void setFocus(Client *client);
    void reloadConfig();
    Layer activeLayer() { return _active_layer; }
    void setActiveLayer(Layer layer) {
        _active_layer = layer;
    }
    const ShortcutSet *mainShortcuts();
    Mode *mode(size_t index) {
        assert(index < _modes.size());
        return _modes[index];
    }
    size_t defaultMode() { return _default_mode; }
    size_t numModes() { return _modes.size(); }

    static Application *self() { return _self; }

    // helper functions
    static Workspace *activeWorkspace();
    static void manageClient(WidgetBackend *backend, bool is_floating);
    static void unmanageClient(Widget *frontend);
    static void runProgram(const char *path);
    static void focusActiveClient();
//     static Client *activeClient();
    static Widget *activeClient();
    static void makeClientActive(Widget *client);

protected:
    Application();

    void init();
    void shutdown();
    
private:
    ActionSet *_main_actions = 0;
    std::vector<Mode*> _modes;
    size_t _default_mode = 0;
    Layer _active_layer = LAYER_FLOATING;

    static Application *_self;
};

#endif // __APPLICATION_H__