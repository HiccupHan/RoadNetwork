#pragma once

#include <string>
#include <unordered_map>
using namespace std;

class RoadSegment {
    public:
        RoadSegment(const string &name_, const string &start_intersection_, const string &end_intersection_, int length_, int num_lanes_);
        int getLength() const;
        int getPOIStart(const string& poi) const;
        int getPOIEnd(const string& poi) const;
        void addPOI(const string& poi, int loc);
        string getStartIntersection() const;
        string getEndIntersection() const;
    private:
        string name;
        string start_intersection;
        string end_intersection;
        int length;
        int num_lanes;
        unordered_map<string, pair<int, int>> POIs_loc;
};
