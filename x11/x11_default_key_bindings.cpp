#include "x11_default_key_bindings.h"

#include "x11_application.h"
#include "x11_container_container.h"
#include "client_container.h"
#include "workspace.h"
#include "container_layout.h"
#include "actions.h"
#include "common.h"


struct X11DefaultKeyBindings::Actions
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
        rootContainer()->getLayout()->layoutContents();
    }
    static void rotate()
    {
        Container::rotateOrientation();
        rootContainer()->getLayout()->layoutContents();
    }
#if 0
    static void focusLeft()
    {
        clientContainer()->focusSibling(Container::WEST);
    }
    static void focusRight()
    {
        clientContainer()->focusSibling(Container::EAST);
    }
    static void focusUp()
    {
        clientContainer()->focusSibling(Container::NORTH);
    }
    static void focusDown()
    {
        clientContainer()->focusSibling(Container::SOUTH);
    }
#endif
    static void runTerminal()
    {
        X11Application::runProgram("/usr/bin/xterm");
    }
    static void redraw()
    {
        rootContainer()->redrawAll();
    }

#if 0
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
#endif
    static void runProgram()
    {
        X11Application::runProgram("/usr/bin/gmrun");
    }
    static void toggleMaximize()
    {
        debug;
        bool maximized = X11Application::activeWorkspace()->maximized();
        X11Application::activeWorkspace()->setMaximized(!maximized);
    }
    static void toggleExpanding()
    {
        if (ClientContainer *c = clientContainer())
            c->enableFixedSize(!c->isFixedSize());
    }
    static void toggleParentExpanding()
    {
        if (ClientContainer *c = clientContainer())
            c->parent()->enableFixedSize(!c->parent()->isFixedSize());
    }
    static void incExtraSpace()
    {
//         clientContainer()->parent()->incAvailableSpacePortion(clientContainer(), 100);
    }
    static void decExtraSpace()
    {
//         clientContainer()->parent()->decAvailableSpacePortion(clientContainer(), 100);
//         clientContainer()->decCustomSize();
    }
    static void resetExtraSpace()
    {
//         
//         clientContainer()->setCustomSizeActive(false);
    }


    static void incWidth()
    {
        changeSize(true, 100);
    }
    static void decWidth()
    {
        changeSize(true, -100);
    }
    static void incHeight()
    {
        changeSize(false, 100);
    }
    static void decHeight()
    {
        changeSize(false, -100);
    }

    static void changeSize(bool horizontal, int delta)
    {
        if (Container *c = clientContainer()) {
            Container *target = (c->parent()->isHorizontal() == horizontal) ? c : c->parent();
            if (target->isFixedSize()) {
                if (horizontal)
                    target->setFixedWidth(target->fixedWidth() + delta);
                else
                    target->setFixedHeight(target->fixedHeight() + delta);
            }
        }
    }

#if 0
    //HACK
    static void addNewClientContainer()
    {
        rootContainer()->addNewClientContainer(false);
    }
#endif
};


X11DefaultKeyBindings::X11DefaultKeyBindings()
{
    createShortcut("l", Mod1Mask, &Actions::layout);
    createShortcut("r", Mod1Mask, &Actions::rotate);
//     createShortcut("KP_Left", Mod1Mask, &Actions::focusLeft);
//     createShortcut("KP_Right", Mod1Mask, &Actions::focusRight);
//     createShortcut("KP_Up", Mod1Mask, &Actions::focusUp);
//     createShortcut("KP_Down", Mod1Mask, &Actions::focusDown);
    createShortcut("t", Mod1Mask, &Actions::runTerminal);
    createShortcut("d", Mod1Mask, &Actions::redraw);
//     createShortcut("comma", Mod1Mask, &Actions::focusPrevClient);
//     createShortcut("period", Mod1Mask, &Actions::focusNextClient);
    createShortcut("KP_Left", Mod1Mask | ShiftMask, &::Actions::moveClientLeft);
    createShortcut("KP_Right", Mod1Mask | ShiftMask, &::Actions::moveClientRight);
    createShortcut("KP_Up", Mod1Mask | ShiftMask, &::Actions::moveClientUp);
    createShortcut("KP_Down", Mod1Mask | ShiftMask, &::Actions::moveClientDown);
//     createShortcut("c", Mod1Mask, &Actions::deleteEmptyContainers);
    createShortcut("F2", Mod1Mask, &Actions::runProgram);
//     createShortcut("Return", Mod1Mask, &Actions::toggleMaximize);

    createShortcut("m", Mod1Mask, &Actions::toggleMaximize);
    createShortcut("e", Mod1Mask, &Actions::toggleExpanding);
    createShortcut("e", ControlMask | Mod1Mask, &Actions::toggleParentExpanding);

    createShortcut("KP_Right", ControlMask | Mod1Mask, &Actions::incWidth);
    createShortcut("KP_Left", ControlMask | Mod1Mask, &Actions::decWidth);
    createShortcut("KP_Down", ControlMask | Mod1Mask, &Actions::incHeight);
    createShortcut("KP_Up", ControlMask | Mod1Mask, &Actions::decHeight);


    //HACK
//     createShortcut("n", Mod1Mask, &Actions::addNewClientContainer);
}
