#include "x11_default_key_handler.h"

#include "x11_application.h"
#include "x11_container_container.h"
#include "client_container.h"


struct X11DefaultKeyHandler::Actions
{
    static X11ContainerContainer *rootContainer()
    {
        return X11Application::activeRootContainer();
    }
    static ClientContainer *clientContainer()
    {
        return rootContainer()->activeClientContainer();
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
        clientContainer()->focusSilbling(Container::WEST);
    }
    static void focusRight()
    {
        clientContainer()->focusSilbling(Container::EAST);
    }
    static void focusUp()
    {
        clientContainer()->focusSilbling(Container::NORTH);
    }
    static void focusDown()
    {
        clientContainer()->focusSilbling(Container::SOUTH);
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
        clientContainer()->focusPrevClient();
    }
    static void focusNextClient()
    {
        clientContainer()->focusNextClient();
    }
    static void moveClientLeft()
    {
        clientContainer()->moveClient(Container::WEST);
    }
    static void moveClientRight()
    {
        clientContainer()->moveClient(Container::EAST);
    }
    static void moveClientUp()
    {
        clientContainer()->moveClient(Container::NORTH);
    }
    static void moveClientDown()
    {
        clientContainer()->moveClient(Container::SOUTH);
    }
    static void deleteEmptyContainers()
    {
        rootContainer()->deleteEmptyChildren();
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
    createShortcut("KP_Left", Mod1Mask | ShiftMask, &Actions::moveClientLeft);
    createShortcut("KP_Right", Mod1Mask | ShiftMask, &Actions::moveClientRight);
    createShortcut("KP_Up", Mod1Mask | ShiftMask, &Actions::moveClientUp);
    createShortcut("KP_Down", Mod1Mask | ShiftMask, &Actions::moveClientDown);
    createShortcut("c", Mod1Mask, &Actions::deleteEmptyContainers);
}
