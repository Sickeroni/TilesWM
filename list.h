#ifndef __LIST_H__
#define __LIST_H__

#include <stdlib.h>


class ListBase
{
protected:
    class Private;

    class Item {
        friend struct ListBase::Private;

    public:
        Item() : _prev(0), _next(0) {}

        Item *prev() { return _prev; }
        Item *next() { return _next; }

        bool isUnlinked() { return !_prev && !_next; }

    private:
        Item *_prev, *_next;
    };

    class Private
    {
    public:
        Private() : _first(0), _last(0) {}

        void append(Item *item);
        void prepend(Item *item);
        void remove(Item *item);
        void replace(Item *old_item, Item *new_item);

        bool isSane() {
            return ((_first && _last) || (!_first && !_last));
        }

        bool isEmpty()
        {
            //ASSERT(isSane());
            return !_first;
        }

    private:
        Item *_first;
        Item *_last;
    } d;
};


template <class T>
class List : public ListBase
{
public:
    class Item : public ListBase::Item
    {
    public:
        T *prev() {
            return static_cast<T*>(ListBase::Item::prev());
        }
        T *next() {
            return static_cast<T*>(ListBase::Item::next());
        }
    };

    bool isEmpty() {
        return d.isEmpty();
    }
    void prepend(T *item) {
        d.prepend(item);
    }
    void append(T *item) {
        d.append(item);
    }
    void remove(T *item) {
        d.remove(item);
    }
    void replace(T *old_item, T *new_item) {
        d.replace(old_item, new_item);
    }
};


#endif // __LIST_H__
