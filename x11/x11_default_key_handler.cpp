#include "x11_default_key_handler.h"

#include "x11_application.h"
#include "x11_container_container.h"
#include "client_container.h"


struct X11DefaultKeyHandler::Actions
{
    static X11ContainerContainer *rootContainer() {
        return X11Application::activeRootContainer();
    }

    static void layout()
    {
        rootContainer()->layout();
    }
    static void rotate()
    {
        Container::rotateOrientation();
        rootContainer()->layout();
    }
    static void focusLeft()
    {
        rootContainer()->activeClientContainer()->focusSilbling(Container::WEST);
    }
    static void focusRight()
    {
        rootContainer()->activeClientContainer()->focusSilbling(Container::EAST);
    }
    static void focusUp()
    {
        rootContainer()->activeClientContainer()->focusSilbling(Container::NORTH);
    }
    static void focusDown()
    {
        rootContainer()->activeClientContainer()->focusSilbling(Container::SOUTH);
    }
    static void runTerminal()
    {
        X11Application::runProgram("/usr/bin/xterm");
    }
    static void redraw()
    {
        rootContainer()->redrawAll();
    }
    static void focusPrevClient()
    {
        rootContainer()->activeClientContainer()->focusPrevClient();
    }
    static void focusNextClient()
    {
        rootContainer()->activeClientContainer()->focusNextClient();
    }
};


X11DefaultKeyHandler::X11DefaultKeyHandler()
{
    createShortcut("l", Mod1Mask, &Actions::layout);
    createShortcut("r", Mod1Mask, &Actions::rotate);
    createShortcut("KP_Left", Mod1Mask, &Actions::focusLeft);
    createShortcut("KP_Right", Mod1Mask, &Actions::focusRight);
    createShortcut("KP_Up", Mod1Mask, &Actions::focusUp);
    createShortcut("KP_Down", Mod1Mask, &Actions::focusDown);
    createShortcut("t", Mod1Mask, &Actions::runTerminal);
    createShortcut("d", Mod1Mask, &Actions::redraw);
    createShortcut("comma", Mod1Mask, &Actions::focusPrevClient);
    createShortcut("period", Mod1Mask, &Actions::focusNextClient);
}
