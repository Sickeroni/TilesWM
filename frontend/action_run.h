#ifndef __ACTION_RUN__
#define __ACTION_RUN__

#include "common_complex_action.h"

class ActionRun final : public CommonComplexAction
{
public:
    virtual ComplexAction::Parameters *parseParameters(const std::vector<std::string> &parameters) const override;
    virtual void exec(const ComplexAction::Parameters *parameters) const override;

private:
    struct Parameters : public ComplexAction::Parameters {
        std::vector<std::string> args;
    };
};

#endif
