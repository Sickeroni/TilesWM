#ifndef __APPLICATION_H__
#define __APPLICATION_H__

class Workspace;
class ContainerContainer;
class ClientContainer;
class Client;
class Monitor;
class ShortcutSet;

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

    Layer activeLayer() { return LAYER_TILED; } // HACK
    void setActiveLayer(Layer layer) {
        //FIXME
    }
    ShortcutSet *mainShortcuts() { return _main_shortcuts; }

    static Application *self() { return _self; }

    // helper functions
    static Workspace *activeWorkspace();
    static ClientContainer *activeClientContainer();
    static void tileClient(Client *client);
    static void runProgram(const char *path);

protected:
    Application();

    virtual ShortcutSet *createShortcutSet() = 0;

    void initShortcuts();

    ShortcutSet *_main_shortcuts = 0;

    static Application *_self;
};

#endif // __APPLICATION_H__
