#ifndef __MODE_3PANEL_H__
#define __MODE_3PANEL_H__

#include "mode.h"

class ClientContainer;

class Mode3Panel final : public Mode
{
public:
    Mode3Panel();

    virtual WindowManager *createWindowManager(Workspace *workspace) override;
};

#endif
