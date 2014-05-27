#ifndef __ACTIONS_H__
#define __ACTIONS_H__

namespace Actions
{
    void runTerminal();
    void runProgram();

    void toggleMaximize();
    void toggleExpanding();
    void toggleParentExpanding();

    void incExtraSpace();
    void decExtraSpace();
    void resetExtraSpace();

    void incWidth();
    void decWidth();
    void incHeight();
    void decHeight();

    void moveClientLeft();
    void moveClientRight();
    void moveClientUp();
    void moveClientDown();

    void focusPrevClient();
    void focusNextClient();

    void focusLeft();
    void focusRight();
    void focusUp();
    void focusDown();

    void deleteEmptyContainers();

    void redraw();
    void layout();
    void rotate();

    void quit();
}

#endif
