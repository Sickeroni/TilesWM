#ifndef __X11_APPLICATION_H__
#define __X11_APPLICATION_H__

#include <X11/Xlib.h>

#include <string>
#include <map>

#if 1

class X11ContainerContainer;
class X11ShortcutSet;
class Workspace;

class X11Application
{
public:
    static X11Application *self() { return _self; }
    static Display *dpy()  { return self()->_dpy; }
    static Window root()  { return self()->_root; }
    static X11ContainerContainer *activeRootContainer();
    static Workspace *activeWorkspace() { return self()->_workspace; }
    static void runProgram(const char *path);

    static const char *eventTypeToString(size_t id);
    static const char *errorCodeToString(size_t error_code);

    X11Application();

    bool init();
    void shutdown();
    void eventLoop();
    void grabServer();
    void ungrabServer();
    Atom atom(const char *name);

private:
    static void quit(int signum);

    static X11Application *_self;

    Display *_dpy;
    Window _root;
    X11ShortcutSet *_shortcuts;
    int _num_server_grabs;
    volatile bool _quit_requested;
    Workspace *_workspace;
    std::map<std::string, Atom> _atoms; // TODO - use hash
};

#define ATOM(name) (X11Application::self()->atom(#name))


#endif

#endif // __X11_APPLICATION_H__
