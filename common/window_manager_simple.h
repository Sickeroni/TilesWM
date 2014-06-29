#ifndef __WINDOW_MANAGER_SIMPLE_H__
#define __WINDOW_MANAGER_SIMPLE_H__

#include "window_manager.h"

class WindowManagerSimple : public WindowManager
{
public:
    WindowManagerSimple(Workspace *workspace, std::string action_set_name) :
        WindowManager(workspace, action_set_name) {}

    // ShortcutSet::Handler implementation
    virtual void handleShortcut(int id) override {}

    // ActionSet implementation
    virtual void initShortcuts() override {}

    virtual Client *activeClient() { return 0; }
    virtual void manageClient(Client *client) override {}
    virtual void layout() override {}
    virtual void makeClientActive(Widget *client) {}
};

#endif
