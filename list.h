#ifndef __LIST_H__
#define __LIST_H__

#include <stdlib.h>


template <class T>
class List
{
public:
    class Item {
        friend class List<T>;

    public:
        Item() : _prev(0), _next(0) {}

        T *prev() { return _prev; }
        T *next() { return _next; }
        bool isUnlinked() { return !_prev && !_next; }

    private:
        T* _prev, *_next;
    };

    bool isEmpty()
    {
        //ASSERT(isSane());
        return !_first;
    }

    void prepend(T *item);
    void append(T *item)
    {
        //ASSERT(isSane());
        abort();
    }

    void replace(T *old_item, T *new_item);

    void remove(T *item)
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
   }

private:
    bool isSane() {
        return ((_first && _last) || (!_first && !_last));
    }

    T *_first;
    T *_last;
};


#endif // __LIST_H__
