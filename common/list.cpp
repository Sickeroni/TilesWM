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
    assert(!item->isUnlinked());

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
    assert(isSane());
    assert(!old_item->isUnlinked());
    assert(new_item->isUnlinked());

    Item *prev = old_item->_prev;
    Item *next = old_item->_next;

    old_item->_prev = 0;
    old_item->_next = 0;

    if (prev) {
        prev->_next = new_item;
        new_item->_prev = prev;
    }
    if (next) {
        next->_prev = new_item;
        new_item->_next = next;
    }

    if (old_item == _first)
        _first = new_item;
    if (old_item == _last)
        _last = new_item;
}
