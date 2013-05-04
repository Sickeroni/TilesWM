#include "x11_default_key_handler.h"

#include "x11_application.h"
#include "x11_container_container.h"
#include "client_container.h"
#include "workspace.h"

#include <iostream>


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
        //rootContainer()->widget()->unmap(); //HACK
        clientContainer()->moveClient(Container::WEST);
//         rootContainer()->deleteEmptyChildren();
        //rootContainer()->widget()->map(); //HACK
        rootContainer()->setFocus(); //HACK
    }
    static void moveClientRight()
    {
        //rootContainer()->widget()->unmap(); //HACK
        clientContainer()->moveClient(Container::EAST);
//         rootContainer()->deleteEmptyChildren();
        //rootContainer()->widget()->map(); //HACK
        rootContainer()->setFocus(); //HACK
    }
    static void moveClientUp()
    {
        //rootContainer()->widget()->unmap(); //HACK
        clientContainer()->moveClient(Container::NORTH);
//         rootContainer()->deleteEmptyChildren();
        //rootContainer()->widget()->map(); //HACK
        rootContainer()->setFocus(); //HACK
    }
    static void moveClientDown()
    {
        //rootContainer()->widget()->unmap(); //HACK
        clientContainer()->moveClient(Container::SOUTH);
//         rootContainer()->deleteEmptyChildren();
        //rootContainer()->widget()->map(); //HACK
        rootContainer()->setFocus(); //HACK
    }
    static void deleteEmptyContainers()
    {
        rootContainer()->deleteEmptyChildren();
        rootContainer()->setFocus(); //HACK
    }
    static void runProgram()
    {
        X11Application::runProgram("/usr/bin/gmrun");
    }
    static void toggleMaximize()
    {
        std::cout<<"toggleMaximize()\n";
        bool maximized = X11Application::activeWorkspace()->maximized();
        X11Application::activeWorkspace()->setMaximized(!maximized);
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
    createShortcut("F2", Mod1Mask, &Actions::runProgram);
//     createShortcut("Return", Mod1Mask, &Actions::toggleMaximize);
    createShortcut("m", Mod1Mask, &Actions::toggleMaximize);
}
