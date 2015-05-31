#ifndef __X11_APPLICATION_H__
#define __X11_APPLICATION_H__

#include "x11_global.h"
#include "frontend_base.h"
#include "backend.h"

#include <X11/Xlib.h>

#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include <list>

class X11ContainerContainer;
class X11ClientContainer;
class X11Client;
class X11GraphicsSystem;
class FrontendBase;
class WidgetBackend;

class X11Application final : public Backend
{
public:
    X11Application(FrontendBase *frontend);
    ~X11Application();

#if 0
    virtual Monitor *activeMonitor() override;
    virtual void setActiveMonitor(Monitor *monitor) override;
    virtual Workspace *createWorkspace() override;
#endif

    virtual void requestQuit() override {
        _quit_requested = true;
    }
    virtual WidgetBackend *createWidgetBackend() override;
    virtual void getMonitorSize(int &w, int &h) override;
    virtual Icon *loadImage(std::string filename) override;
    virtual AbstractKeySequence *parseKeySequence(std::string sequence) override;
    virtual bool addKeyGrab(const AbstractKeySequence *key_sequence) override;
    virtual void releaseKeyGrab(const AbstractKeySequence *key_sequence) override;


    bool init();
    void shutdown();
    void eventLoop();
    void grabServer();
    void ungrabServer();
    bool addKeyGrab(const X11Global::KeySequence &key_sequence);
    void releaseKeyGrab(const X11Global::KeySequence &key_sequence);
    Atom atom(const char *name);
    Cursor getCursor(unsigned int shape);

    static X11Application *self() { return _self; }
    static Display *dpy()  { return self()->_dpy; }
    static Window root()  { return self()->_root; }
//     static Workspace *activeWorkspace();
    static X11GraphicsSystem *graphicsSystem() { return self()->_graphics_system; }
    static FrontendBase *frontend() { return self()->_frontend; }

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
//     Monitor *_monitor;
//     std::vector<Workspace*>  _workspaces;
    std::map<std::string, Atom> _atoms; // TODO - use hash
    std::list<KeyGrab> _key_grabs;
    std::unordered_map<unsigned int, Cursor> _cursors;
    X11Global::ModMask num_lock_mask = Mod2Mask; //FIXME
    X11GraphicsSystem *_graphics_system = 0;
    FrontendBase *_frontend = 0;
    int _root_width = 0;
    int _root_height = 0;
};

#define ATOM(name) (X11Application::self()->atom(#name))

#endif // __X11_APPLICATION_H__
