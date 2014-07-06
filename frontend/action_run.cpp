#include "action_run.h"
#include "application.h"
#include "common.h"

ComplexAction::Parameters *ActionRun::parseParameters(const std::vector<std::string> &parameters) const
{
    if (!parameters.empty()) {
        Parameters *p = new Parameters;
        p->args = parameters;
        return p;
    } else {
        debug<<"no parameters for action run";
        return 0;
    }
}

void ActionRun::exec(const ComplexAction::Parameters *parameters) const
{
    if (parameters) {
        const Parameters *p = dynamic_cast<const Parameters*>(parameters);
        Application::runProgram(p->args);
    }
}
