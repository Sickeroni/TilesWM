#include "application.h"

#include "monitor.h"
#include "workspace.h"
#include "client_container.h"
#include "container_container.h"
#include "shortcut_set.h"
#include "actions.h"
#include "client.h"
#include "common.h"

#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <X11/Xlib.h> //HACK

Application *Application::_self = 0;

Application::Application()
{
    if (_self)
        abort();
    _self = this;
}

Application::~Application()
{
    _self = 0;
    delete _main_shortcuts;
    _main_shortcuts = 0;
}

Workspace *Application::activeWorkspace()
{
    return self()->activeMonitor()->workspace();
}

ClientContainer *Application::activeClientContainer()
{
    if (self()->activeLayer() != LAYER_TILED)
        return 0;
    else
        return activeWorkspace()->rootContainer()->activeClientContainer();
}

void Application::tileClient(Client *client)
{
    assert(!client->container());
    assert(!client->workspace());

    ContainerContainer *root_container = activeWorkspace()->rootContainer();

    if (!root_container->activeClientContainer()) {
        ClientContainer *c = self()->createClientContainer();
        int index = root_container->addChild(c);
        root_container->setActiveChild(index);
    }

    root_container->activeClientContainer()->addChild(client);
}

void Application::runProgram(const char *path)
{
    char *const args[] = {
        const_cast<char*>(path),
        0
    };

    pid_t pid = fork();

    if (pid > 0) {
        waitpid(pid, 0, WNOHANG);
    } else if (pid == 0) {
        execv(path, args);
        perror ("ERROR: Can't execute program: ");
        exit(1);
    } else
        cerr<<"ERROR: running "<<path<<": Can't fork.";
}

void Application::initShortcuts()
{
    _main_shortcuts = createShortcutSet();

    _main_shortcuts->createShortcut("l", Mod1Mask, &Actions::layout);
    _main_shortcuts->createShortcut("r", Mod1Mask, &Actions::rotate);
    _main_shortcuts->createShortcut("KP_Left", Mod1Mask, &Actions::focusLeft);
    _main_shortcuts->createShortcut("KP_Right", Mod1Mask, &Actions::focusRight);
    _main_shortcuts->createShortcut("KP_Up", Mod1Mask, &Actions::focusUp);
    _main_shortcuts->createShortcut("KP_Down", Mod1Mask, &Actions::focusDown);
    _main_shortcuts->createShortcut("t", Mod1Mask, &Actions::runTerminal);
    _main_shortcuts->createShortcut("d", Mod1Mask, &Actions::redraw);
    _main_shortcuts->createShortcut("comma", Mod1Mask, &Actions::focusPrevClient);
    _main_shortcuts->createShortcut("period", Mod1Mask, &Actions::focusNextClient);
    _main_shortcuts->createShortcut("KP_Left", Mod1Mask | ShiftMask, &Actions::moveClientLeft);
    _main_shortcuts->createShortcut("KP_Right", Mod1Mask | ShiftMask, &Actions::moveClientRight);
    _main_shortcuts->createShortcut("KP_Up", Mod1Mask | ShiftMask, &Actions::moveClientUp);
    _main_shortcuts->createShortcut("KP_Down", Mod1Mask | ShiftMask, &Actions::moveClientDown);
    _main_shortcuts->createShortcut("c", Mod1Mask, &Actions::deleteEmptyContainers);
    _main_shortcuts->createShortcut("F2", Mod1Mask, &Actions::runProgram);

    _main_shortcuts->createShortcut("m", Mod1Mask, &Actions::toggleMaximize);
    _main_shortcuts->createShortcut("e", Mod1Mask, &Actions::toggleExpanding);
    _main_shortcuts->createShortcut("e", ControlMask | Mod1Mask, &Actions::toggleParentExpanding);

    _main_shortcuts->createShortcut("KP_Right", ControlMask | Mod1Mask, &Actions::incWidth);
    _main_shortcuts->createShortcut("KP_Left", ControlMask | Mod1Mask, &Actions::decWidth);
    _main_shortcuts->createShortcut("KP_Down", ControlMask | Mod1Mask, &Actions::incHeight);
    _main_shortcuts->createShortcut("KP_Up", ControlMask | Mod1Mask, &Actions::decHeight);
}
