#ifndef __ACTIONS_H__
#define __ACTIONS_H__

namespace Actions
{
    void runTerminal();
    void runProgram();

    void toggleMaximize();

    void focusPrevClient();
    void focusNextClient();

    void focusLeft();
    void focusRight();
    void focusUp();
    void focusDown();

    void redraw();
    void layout();
    void rotate();
    void changeMode();

    void closeActiveClient();

    void quit();
}

#endif
