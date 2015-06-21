#ifndef __WINDOW_MANAGER_H__
#define __WINDOW_MANAGER_H__

#include "key_grab_handler.h"
#include "mode.h"

#include <string>
#include <vector>
#include <list>

class Client;
class Workspace;
class Widget;
class Mode;
class KeyBindingSet;
class ClientWrapper;

class WindowManager : public KeyGrabHandler
{
public:
    WindowManager(Widget *parent_widget, Mode *mode) :
        _parent_widget(parent_widget),
        _mode(mode) {}
    virtual ~WindowManager() {}

    virtual void layout() = 0; //FIXME - remove ? it should be up to the window manager when to to this
    virtual ClientWrapper *activeClient() = 0;
    virtual void redrawAll() = 0;
    // a wm might be interested whether it has focus
    virtual void setHasFocus(bool has_focus) = 0;
    virtual void manageClient(ClientWrapper *client) = 0;
    virtual void unmanageClient(ClientWrapper *client) = 0;
    virtual void makeClientActive(ClientWrapper *client) = 0;

    virtual void handleParentWidgetSizeChanged() {}

    virtual void setActive(bool active) {
        _is_active = active;
    }

    bool isActive() { return _is_active; }

protected:
    virtual const KeyBindingSet *keyBindings() override { return _mode->keyBindings(); }

    Widget *parentWidget() { return _parent_widget; }

private:
    Widget *_parent_widget = 0;
    Mode *_mode = 0;
    bool _is_active = false;
};

#endif
