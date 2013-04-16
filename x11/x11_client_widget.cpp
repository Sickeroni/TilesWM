#include "x11_client_widget.h"

#include "x11_client.h"


X11ClientWidget::X11ClientWidget(Window wid) : X11Widget(wid, CLIENT),
    _client(new X11Client(this))
{
}

X11ClientWidget::~X11ClientWidget()
{
    delete _client;
    _client = 0;
}

void X11ClientWidget::onMapStateChanged()
{
    _client->onMapStateChanged();
}
