#include <string>
#include "POI.h"

POI::POI(const string &name_, const string &road_) {
    name = name_;
    road = road_;
}

string POI::getName() {
    return name;
}

string POI::getRoad() {
    return road;
}
