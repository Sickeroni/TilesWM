#ifndef __X11_APPLICATION_H__
#define __X11_APPLICATION_H__

#include "application.h"
#include "x11_global.h"

#include <X11/Xlib.h>

#include <string>
#include <map>
#include <vector>
#include <list>

class X11ContainerContainer;
class X11ClientContainer;
class X11Client;
class Workspace;
class Monitor;
class X11GraphicsSystem;

class X11Application final : public Application
{
public:
    X11Application();
    ~X11Application();

    virtual Monitor *activeMonitor() override;
    virtual Workspace *createWorkspace() override;
    virtual void setActiveMonitor(Monitor *monitor) override;
    virtual void setFocus(Client *client) override;
    virtual ShortcutSet *createShortcutSet(std::string name) override;
    virtual void requestQuit() override {
        _quit_requested = true;
    }
    virtual ContainerWidget *createContainerWidget(ContainerBase *container) override;

    bool init();
    void shutdown();
    void eventLoop();
    void grabServer();
    void ungrabServer();
    bool addKeyGrab(const X11Global::KeySequence &key_sequence);
    void releaseKeyGrab(const X11Global::KeySequence &key_sequence);
    Atom atom(const char *name);

    static X11Application *self() { return _self; }
    static Display *dpy()  { return self()->_dpy; }
    static Window root()  { return self()->_root; }
    static Workspace *activeWorkspace();
    static X11Client *activeClient();
    static X11GraphicsSystem *graphicsSystem() { return self()->_graphics_system; }

    static const char *eventTypeToString(size_t id);
    static const char *errorCodeToString(size_t error_code);

private:
    struct KeyGrab
    {
        KeyGrab(X11Global::KeySequence key_sequence) : key_sequence(key_sequence) {}

        X11Global::KeySequence key_sequence;
        int ref_count = 1;
    };

    static void quit(int signum);

    bool handleKeyPress(const XKeyEvent &ev);

    static X11Application *_self;

    Display *_dpy;
    Window _root;
    int _num_server_grabs;
    volatile bool _quit_requested;
    Monitor *_monitor;
    std::vector<Workspace*>  _workspaces;
    std::map<std::string, Atom> _atoms; // TODO - use hash
    std::list<KeyGrab> _key_grabs;
    X11Global::ModMask num_lock_mask = Mod2Mask; //FIXME
    X11GraphicsSystem *_graphics_system = 0;
};

#define ATOM(name) (X11Application::self()->atom(#name))

#endif // __X11_APPLICATION_H__
