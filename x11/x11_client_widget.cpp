#include "x11_client_widget.h"

#include "x11_container_container.h"
#include "x11_client.h"
#include "x11_application.h"

#include <iostream>


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

void X11ClientWidget::newClientWidget(Window wid)
{
    XWindowAttributes attr;
    if (XGetWindowAttributes(X11Application::display(), wid, &attr)) {
        if (attr.override_redirect) // dont't manage popups etc. //FIXME - else warning on client destroy
            return;

        XAddToSaveSet(X11Application::display(), wid);

        X11ClientWidget *widget = new X11ClientWidget(wid);

        X11Application::activeRootContainer()->addClient(widget->client());
    } else {
        std::cerr << "XGetWindowAttributes() for client window " << wid << "failed\n";
    }
}
