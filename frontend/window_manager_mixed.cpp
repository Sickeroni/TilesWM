#include "window_manager_mixed.h"
#include "client_wrapper.h"

WindowManagerMixed::WindowManagerMixed(
        Workspace *workspace,
        Mode *mode,
        Mode *tiling_mode,
        Mode *floating_mode) :
    WindowManager(workspace, mode)
{
    _tiling_wm = tiling_mode->createWindowManager(workspace);
    _floating_wm = floating_mode->createWindowManager(workspace);
}

WindowManagerMixed::~WindowManagerMixed()
{
    _wm_of_client.clear();
    delete _tiling_wm;
    _tiling_wm = 0;
    delete _floating_wm;
    _floating_wm = 0;
}

void WindowManagerMixed::layout()
{
    _tiling_wm->layout();
    _floating_wm->layout();
}

ClientWrapper *WindowManagerMixed::activeClient()
{
    //FIXME
    return _tiling_wm->activeClient();
}

void WindowManagerMixed::handleWorkspaceSizeChanged()
{
    _tiling_wm->handleWorkspaceSizeChanged();
    _floating_wm->handleWorkspaceSizeChanged();
}

void WindowManagerMixed::redrawAll()
{
    _tiling_wm->redrawAll();
    _floating_wm->redrawAll();
}

void WindowManagerMixed::setHasFocus(bool has_focus)
{
    _tiling_wm->setHasFocus(has_focus);
    _floating_wm->setHasFocus(has_focus);
}

void WindowManagerMixed::setActive(bool active)
{
    WindowManager::setActive(active);
    _tiling_wm->setActive(active);
    _floating_wm->setActive(active);
}

bool WindowManagerMixed::handleKeySequence(const AbstractKeySequence *sequence)
{
    //FIXME
    bool handled = _tiling_wm->handleKeySequence(sequence);
    if (!handled)
        handled = _floating_wm->handleKeySequence(sequence);
    return handled;
}

void WindowManagerMixed::manageClient(ClientWrapper *client)
{
    if (client->wrappedClient()->isDialog() || client->wrappedClient()->isModal()) {
        _floating_wm->manageClient(client);
        _wm_of_client[client] = _floating_wm;
    } else {
        _tiling_wm->manageClient(client);
        _wm_of_client[client] = _tiling_wm;
    }
}

void WindowManagerMixed::unmanageClient(ClientWrapper *client)
{
    WindowManager *wm = _wm_of_client[client];
    assert(wm);
    auto it = _wm_of_client.find(client);
    assert(it != _wm_of_client.end());
    if (it != _wm_of_client.end()) {
        WindowManager *wm = it->second;
        _wm_of_client.erase(it);
        wm->unmanageClient(client);
    }
}

void WindowManagerMixed::makeClientActive(ClientWrapper *client)
{
    WindowManager *wm = _wm_of_client[client];
    assert(wm);
    wm->makeClientActive(client);
}

void WindowManagerMixed::performAction(int id)
{
    (void)id;
    abort();
}

void WindowManagerMixed::performComplexAction(const ComplexAction *action, const ComplexAction::Parameters *parameters)
{
    (void)action;
    (void)parameters;
    abort();
}
