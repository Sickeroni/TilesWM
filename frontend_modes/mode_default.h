#ifndef __MODE_DEFAULT_H__
#define __MODE_DEFAULT_H__

#include "mode.h"

class ModeDefault final : public Mode
{
public:
    ModeDefault();

    virtual WindowManager *createWindowManager(Workspace *workspace) override;
};


#endif
