#include "application.h"

#include "common.h"

Application *Application::_self = 0;

Application::Application()
{
    if (_self)
        abort();
    _self = this;
}

Application::~Application()
{
    _self = 0;
}
