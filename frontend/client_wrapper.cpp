#include "client_wrapper.h"
#include "application.h"
#include "backend.h"

ClientWrapper::ClientWrapper(Client *client, WindowManager *wm) : ChildWidget(OTHER),
    _client(client),
    _client_backend(client->backend()),
    _wm(wm)
{
    _rect = _client_backend->rect();
    _backend = Application::self()->backend()->createWidgetBackend();
    _backend->setRect(_rect);

    handleWindowManagerIsActiveChanged();
}

ClientWrapper::~ClientWrapper()
{
    if (_client->parent() == this) {
        _client->setDragHandler(0);
        _client->reparent(0, 0);
    }
}

void ClientWrapper::handleWindowManagerIsActiveChanged()
{
    if (_wm->isActive()) {
        _client->reparent(this, _backend);
        _client->setRect(Rect(0, 0, rect().w, rect().h));
        _client->setDragHandler(_drag_handler);
        _client->setMapped(true);
    }
}

void ClientWrapper::setRect(const Rect &rect)
{
    ChildWidget::setRect(rect);
    if (_wm->isActive())
        _client->setRect(Rect(0, 0, rect.w, rect.h));
}

void ClientWrapper::setDragHandler(Client::DragHandler *handler)
{
    _drag_handler = handler;
    if (_wm->isActive())
        _client->setDragHandler(handler);
}
