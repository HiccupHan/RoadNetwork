#include "POI.h"

POI::POI(const string &name_, const string &road_) {
    name = name_;
    road = road_;
}

string POI::getName() const {
    return name;
}

string POI::getRoad() const {
    return road;
}
