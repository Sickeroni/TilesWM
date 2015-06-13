#ifndef __MODE_MIXED_H__
#define __MODE_MIXED_H__

#include "mode.h"

class ModeMixed : public Mode
{
public:
    // this takes ownership of the passed modes
    ModeMixed(Mode *floating_mode, Mode *tiling_mode);
    ~ModeMixed();

    virtual WindowManager *createWindowManager(Workspace *workspace) override;

    virtual void createKeyBindings() override;
    virtual void clearKeyBindings() override;

private:
    Mode *_floating_mode = 0;
    Mode *_tiling_mode = 0;
};

#endif
