#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "frontend_base.h"
#include "common.h"

#include <vector>
#include <cstddef>
#include <string>

class Workspace;
class Client;
class Monitor;
class ShortcutSet;
class Mode;
class CommonActions;
class WidgetBackend;
class Widget;

class Application final : public FrontendBase
{
public:
    enum Layer
    {
        LAYER_FLOATING,
        LAYER_TILED
    };

    Application();
    ~Application();

    virtual void init(Backend *backend) override;
    virtual void shutdown() override;
    virtual void focusActiveClient() override;
    virtual ClientFrontend *createClientFrontend(ClientBackend *backend, bool is_floating) override;
    virtual void destroyClientFrontend(ClientFrontend *frontend) override;
    virtual int numKeyGrabHandlers() override;
    virtual KeyGrabHandlerBase *keyGrabHandler(int index) override;

    Monitor *activeMonitor() { return _monitor; }
//     void setActiveMonitor(Monitor *monitor);
//     virtual void requestQuit() = 0;
//     virtual WidgetBackend *createWidgetBackend() = 0;
//     virtual ShortcutSet *createShortcutSet(std::string name) = 0;
    Workspace *createWorkspace();

    void setFocus(Client *client);
    void reloadConfig();
    Layer activeLayer() { return _active_layer; }
    void setActiveLayer(Layer layer) {
        _active_layer = layer;
    }
//     const ShortcutSet *mainShortcuts();
    Mode *mode(size_t index) {
        assert(index < _modes.size());
        return _modes[index];
    }
    size_t defaultMode() { return _default_mode; }
    size_t numModes() { return _modes.size(); }

    static Application *self() { return _self; }

    // helper functions
    static Workspace *activeWorkspace();
//     static void manageClient(WidgetBackend *backend, bool is_floating);
//     static void unmanageClient(Widget *frontend);
    static void runProgram(const char *path);
//     static Client *activeClient();
    static Widget *activeClient();
//     static void makeClientActive(Widget *client);
    static Backend *backend() { return self()->_backend; }

private:
//     ActionSet *_main_actions = 0;
    CommonActions *_common_actions = 0;
    std::vector<Mode*> _modes;
    size_t _default_mode = 0;
    Layer _active_layer = LAYER_FLOATING;

    static Application *_self;
    std::vector<Workspace*>  _workspaces;
    Monitor *_monitor = 0;
    Backend *_backend = 0;
};

#endif // __APPLICATION_H__
