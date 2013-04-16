#include "list.h"

void ListBase::Private::prepend(Item *item)
{
    //FIXME
    abort();

    //ASSERT(isSane());

    _count++;
}

void ListBase::Private::append(Item *item)
{
    //ASSERT(isSane());
    //ASSERT(item->isUnlinked());

    if (_last) {
        item->_prev = _last;
        _last->_next = item;
    } else
        _first = _last = item;

    _count++;
}

void ListBase::Private::remove(Item *item)
{
    //ASSERT(isSane());

    if (item->_prev)
        item->_prev->_next = item->_next;
    if (item->_next)
        item->_next->_prev = item->_prev;

    if (_first == item)
        _first = item->_next;
    if (_last == item)
        _last = item->_prev;

    item->_prev = 0;
    item->_next = 0;

    _count--;
}

void ListBase::Private::replace(Item *old_item, Item *new_item)
{
    //FIXME
    abort();
}
