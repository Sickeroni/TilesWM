#ifndef __ACTION_H__
#define __ACTION_H__

#include "common.h"

#include <string>
#include <vector>

class ComplexAction
{
public:
    struct Parameters
    {
        virtual ~Parameters() {}
    };

    virtual ~ComplexAction() {}

    virtual Parameters *parseParameters(const std::vector<std::string> &/*parameters*/) const { return 0; }
};

struct Action
{
    enum Type
    {
        TYPE_SIMPLE,
        TYPE_COMPLEX
    };

    Action(const std::string &_name, int _id) : 
        type(TYPE_SIMPLE),
        name(_name),
        id(_id) {}

    // action becomes owner of _complex_action
    Action(const std::string &_name, ComplexAction *_complex_action) :
        type(TYPE_COMPLEX),
        name(_name),
        complex_action(_complex_action) {}

    ~Action() {
        if (type == TYPE_COMPLEX)
            delete complex_action;
    }

    const Type type = TYPE_SIMPLE;
    const std::string name;
    union {
        ComplexAction *complex_action;
        const int id;
    };
};

#endif
