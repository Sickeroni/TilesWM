#ifndef __COMMON_COMPLEX_ACTION_H__
#define __COMMON_COMPLEX_ACTION_H__

#include "action.h"

class CommonComplexAction : public ComplexAction
{
public:
    virtual void exec(const ComplexAction::Parameters *parameters) const = 0;
};

#endif
