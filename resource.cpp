#include "resource.h"

Resource::Resource()
{
}


bool Resource::operator ==(Resource &other)
{
    if (id != other.id) {
        return false;
    }
    if (type != other.type) {
        return false;
    }
    if (use_type != other.use_type) {
        return false;
    }
    if (price != other.price) {
        return false;
    }
    return true;
}
