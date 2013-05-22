#ifndef __LIST_H__
#define __LIST_H__

#include <assert.h>
#include <stdlib.h>


class ListBase
{
public:
    ~ListBase();

protected:
    class Private;

    class Item {
        friend class ListBase::Private;

    public:
        Item() : _prev(0), _next(0) {}
        ~Item() {
            //FIXME either remove from list or assert we are not part of a list
//             assert(isUnlinked());
        }

        Item *prev() { return _prev; }
        Item *next() { return _next; }

        //FIXME: this was meant to indicate if item belongs to a list
        // _prev and _next are NOT indicators of that when list contains only one item
        // add is_linked flag or point to list ?
//         bool isUnlinked() { return !_prev && !_next; }

    private:
        Item *_prev, *_next;
    };

    class Private
    {
    public:
        Private() : _count(0), _first(0), _last(0) {}

        Item *first() { return _first; }

        int count() { return _count; }

        void append(Item *item);
        void prepend(Item *item);
        void remove(Item *item);
        void replace(Item *old_item, Item *new_item);

        bool isSane() {
            return ((_first && _last) || (!_first && !_last));
        }

        bool isEmpty()
        {
            assert(isSane());
            return !_first;
        }

    private:
        int _count;
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

    int count() {
        return d.count();
    }

    T *first() {
        return static_cast<T*>(d.first());
    }

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
