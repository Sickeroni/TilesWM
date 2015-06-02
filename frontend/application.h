#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "frontend_base.h"
#include "common.h"

#include <vector>
#include <cstddef>
#include <string>

class Workspace;
class Client;
class Monitor;
class ShortcutSet;
class Mode;
class CommonActions;
class WidgetBackend;
class Widget;
class ClientWrapper;
class ThemeBackend;

class Application final : public FrontendBase
{
public:
    Application(const std::vector<Mode*> *modes, ThemeBackend *theme_backend);
    ~Application();

    virtual void init(Backend *backend) override;
    virtual void shutdown() override;
    virtual void focusActiveClient() override;
    virtual ClientFrontend *createClientFrontend(ClientBackend *backend) override;
    virtual void destroyClientFrontend(ClientFrontend *frontend) override;
    virtual bool handleKeySequence(const AbstractKeySequence *sequence) override;

    Monitor *activeMonitor() { return _monitor; }
    void setActiveMonitor(Monitor *monitor);
    Workspace *createWorkspace();

    void reloadConfig();
    Mode *mode(size_t index) {
        assert(index < _modes->size());
        return _modes->at(index);
    }
    size_t defaultMode() { return _default_mode; }
    size_t numModes() { return _modes->size(); }

    static Application *self() { return _self; }

    // helper functions
    static Workspace *activeWorkspace();
    static void manageClient(Client *client);
    static void unmanageClient(Client *client);
    static void runProgram(const char *path);
    static void runProgram(const std::vector<std::string> &args);
    static ClientWrapper *activeClient();
    static Backend *backend() { return self()->_backend; }
    static ThemeBackend *themeBackend() { return self()->_theme_backend; }

private:
//     ActionSet *_main_actions = 0;
    const std::vector<Mode*> *_modes = 0;
    CommonActions *_common_actions = 0;
    size_t _default_mode = 0;

    static Application *_self;
    std::vector<Workspace*>  _workspaces;
    Monitor *_monitor = 0;
    Backend *_backend = 0;
    ThemeBackend *_theme_backend = 0;
};

#endif // __APPLICATION_H__
