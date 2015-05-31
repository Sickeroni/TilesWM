#ifndef __ABSTRACT_KEY_SEQUENCE_H__
#define __ABSTRACT_KEY_SEQUENCE_H__

struct AbstractKeySequence
{
    virtual ~AbstractKeySequence() {}
    virtual bool equals(const AbstractKeySequence *other) const = 0;
};

#endif
