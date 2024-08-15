#include <string>
#include <unordered_map>
#include "RoadSegment.h"
using namespace std;

RoadSegment::RoadSegment(const string &name_, const string &start_intersection_, const string &end_intersection_, const int length_, const int num_lanes_) {
    name = name_;
    length = length_;
    num_lanes = num_lanes_;
    start_intersection = start_intersection_;
    end_intersection = end_intersection_;
}

int RoadSegment::getLength() const{
    return length;
}

int RoadSegment::getPOIEnd(const string& poi) const {
    return POIs_loc.find(poi)->second.second;
}
int RoadSegment::getPOIStart(const string& poi) const {
    return POIs_loc.find(poi)->second.first;
}

void RoadSegment::addPOI(const string& poi, const int loc) {
    POIs_loc[poi].first = loc;
    POIs_loc[poi].second = length - loc;
}

string RoadSegment::getStartIntersection() const {
    return start_intersection;
}

string RoadSegment::getEndIntersection() const {
    return end_intersection;
}

