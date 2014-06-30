#ifndef __MODE_SIMPLE_H__
#define __MODE_SIMPLE_H__

#include "mode.h"

class ModeSimple final : public Mode
{
public:
    ModeSimple();

    virtual WindowManager *createWindowManager(Workspace *workspace) override;
};


#endif
