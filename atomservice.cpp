#include "atomservice.h"

AtomService::AtomService()
{

}

bool AtomService::operator ==(AtomService &other)
{
    if (activityNumber != other.activityNumber) {
        return false;
    }
    if (id != other.id) {
        return false;
    }
    if (redoCount != other.redoCount) {
        return false;
    }
    if (free != other.free) {
        return false;
    }
    if (price != other.price) {
        return false;
    }
    if (reliability != other.reliability) {
        return false;
    }
    if (execTime != other.execTime) {
        return false;
    }
    return true;
}
