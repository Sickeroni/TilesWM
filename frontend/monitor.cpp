#include "monitor.h"

#include "application.h"
#include "workspace.h"
#include "status_bar.h"
#include "rect.h"

Monitor::Monitor() :
    _workspace(Application::self()->createWorkspace()),
    _status_bar(new StatusBar())
{
    _workspace->setMonitor(this);
    _status_bar->setMapped(true);
}

Monitor::~Monitor()
{
    delete _status_bar;
    _status_bar = 0;
    _workspace->setMonitor(0);
    _workspace = 0;
}

void Monitor::setSize(int w, int h)
{
    _w = w;
    _h = h;
    layoutContents();
}

void Monitor::layoutContents()
{
    int status_bar_height = _status_bar->maxTextHeight() + 4;
    _workspace->setRect(Rect(0, status_bar_height, _w, _h - status_bar_height));
    _workspace->layoutContents();
    _status_bar->setRect(Rect(0, 0, _w, status_bar_height));
}

void Monitor::setStatusText(const std::string &text)
{
    _status_bar->setText(text);
}
