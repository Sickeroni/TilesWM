#ifndef __WINDOW_MANAGER_MIXED_H__
#define __WINDOW_MANAGER_MIXED_H__

#include "window_manager.h"

#include <unordered_map>

class WindowManagerMixed : public WindowManager
{
public:
    WindowManagerMixed(Workspace *workspace, Mode *mode, Mode *tiling_mode, Mode *floating_mode);
    ~WindowManagerMixed();

    virtual void layout() override;
    virtual ClientWrapper *activeClient() override;
    virtual void redrawAll() override;
    virtual void setHasFocus(bool has_focus) override;

    virtual void setActive(bool active) override;
    virtual void handleWorkspaceSizeChanged() override;

    virtual bool handleKeySequence(const AbstractKeySequence *sequence) override;

protected:
    virtual void manageClient(ClientWrapper *client) override;
    virtual void unmanageClient(ClientWrapper *client) override;
    virtual void makeClientActive(ClientWrapper *client) override;

    virtual void performAction(int id) override;
    virtual void performComplexAction(const ComplexAction *action, const ComplexAction::Parameters *parameters) override;

private:
    WindowManager *_tiling_wm = 0;
    WindowManager *_floating_wm = 0;
    std::unordered_map<ClientWrapper*, WindowManager*> _wm_of_client;
};

#endif
