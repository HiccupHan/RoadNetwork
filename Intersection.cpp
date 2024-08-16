#include <string>
#include "Intersection.h"
using namespace std;

Intersection::Intersection(const string &name_) {
    name = name_;
}

void Intersection::addConnection(const string& road) {
    connected_segments.insert(road);
}

set<string> Intersection::getConnectedRoads() {
    return connected_segments;
}
