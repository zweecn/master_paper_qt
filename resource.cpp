#include "resource.h"

Resource::Resource()
{
    id = 0;
    type = 0;
    use_type = 0;
    price = 0;
    used = 0;
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
    if (used != other.used) {
        return false;
    }
    return true;
}
