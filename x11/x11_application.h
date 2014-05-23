#ifndef __X11_APPLICATION_H__
#define __X11_APPLICATION_H__

#include "application.h"

#include <X11/Xlib.h>

#include <string>
#include <map>
#include <vector>

#if 1

class X11ContainerContainer;
class X11ShortcutSet;
class X11ClientContainer;
class X11Client;
class Workspace;
class Monitor;

class X11Application : public Application
{
public:
    X11Application();
    ~X11Application();

    virtual Monitor *activeMonitor();
    virtual Workspace *createWorkspace();
    virtual ContainerContainer *createContainerContainer();
    virtual ClientContainer *createClientContainer();
    virtual void setActiveMonitor(Monitor *monitor);

    bool init();
    void shutdown();
    void eventLoop();
    void grabServer();
    void ungrabServer();
    Atom atom(const char *name);

    static X11Application *self() { return _self; }
    static Display *dpy()  { return self()->_dpy; }
    static Window root()  { return self()->_root; }
    static X11ContainerContainer *activeRootContainer();
    static X11ClientContainer *activeClientContainer();
    static Workspace *activeWorkspace();
    static X11Client *activeClient();
    static void unfocusActiveClient(); //FIXME implement

    static const char *eventTypeToString(size_t id);
    static const char *errorCodeToString(size_t error_code);

private:
    static void quit(int signum);

    static X11Application *_self;

    Display *_dpy;
    Window _root;
    X11ShortcutSet *_shortcuts;
    int _num_server_grabs;
    volatile bool _quit_requested;
    Monitor *_monitor;
    std::vector<Workspace*>  _workspaces;
    std::map<std::string, Atom> _atoms; // TODO - use hash
};

#define ATOM(name) (X11Application::self()->atom(#name))


#endif

#endif // __X11_APPLICATION_H__
