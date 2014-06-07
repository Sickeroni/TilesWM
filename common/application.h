#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <vector>
#include <cstddef>
#include <string>

class Workspace;
class ContainerContainer;
class ClientContainer;
class Client;
class Monitor;
class ShortcutSet;
class Mode;
class ActionSet;

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
    virtual Workspace *createWorkspace() = 0;
    virtual ContainerContainer *createContainerContainer() = 0;
    virtual ClientContainer *createClientContainer() = 0;
    virtual void setActiveMonitor(Monitor *monitor) = 0;
    virtual void setFocus(Client *client) = 0;
    virtual ShortcutSet *createShortcutSet(std::string name) = 0;
    virtual void requestQuit() = 0;

    Layer activeLayer() { return LAYER_TILED; } // HACK
    void setActiveLayer(Layer layer) {
        //FIXME
    }
    const ShortcutSet *mainShortcuts();
    Mode *mode(size_t index) { return _modes[index]; }
    size_t defaultMode() { return _default_mode; }
    size_t numModes() { return _modes.size(); }

    static Application *self() { return _self; }

    // helper functions
    static Workspace *activeWorkspace();
    static ClientContainer *activeClientContainer();
    static void manageClient(Client *client, bool is_floating);
    static void unmanageClient(Client *client);
    static void runProgram(const char *path);
    static void focusActiveClient();
    static Client *activeClient();

protected:
    Application();

    void init();
    void shutdown();

    ActionSet *_main_actions = 0;
    std::vector<Mode*> _modes;
    size_t _default_mode = 0;

    static Application *_self;
};

#endif // __APPLICATION_H__
