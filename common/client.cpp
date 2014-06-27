#include "client.h"

#include "common.h"

Client::Client(bool is_mapped) :
    _is_mapped(is_mapped),
    _has_focus(false),
    _name("<no name>"),
    _min_width(0),
    _min_height(0),
    _max_width(0),
    _max_height(0)
{
}

Client::~Client()
{
}
