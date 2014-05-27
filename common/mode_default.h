#ifndef __MODE_DEFAULT_H__
#define __MODE_DEFAULT_H__

#include "mode.h"

class ModeDefault final : public Mode
{
public:
    virtual void activate(Workspace *workspace) override;
    virtual void initShortcuts() override;
};


#endif
