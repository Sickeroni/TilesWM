#include "list.h"

#include <assert.h>

ListBase::~ListBase()
{
    for (Item *item = d.first(); item; ) {
        Item *remove_this = item;
        item = item->next();
        d.remove(remove_this);
    }
}

void ListBase::Private::prepend(Item *item)
{
    assert(isSane());
    assert(item->isUnlinked());

    if (_first) {
        item->_next = _first;
        _first->_prev = item;
        _first = item;
    } else
        _first = _last = item;

    _count++;
}

void ListBase::Private::append(Item *item)
{
    assert(isSane());
    assert(item->isUnlinked());

    if (_last) {
        item->_prev = _last;
        _last->_next = item;
        _last = item;
    } else
        _first = _last = item;

    _count++;
}

void ListBase::Private::remove(Item *item)
{
    assert(isSane());

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

// void ListBase::Private::replace(Item *old_item, Item *new_item)
// {
//     //FIXME
//     abort();
// }
