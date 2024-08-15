#pragma once

#include <vector>
#include "RoadSegment.h"
#include <set>
#include <string>
using namespace std;

class Intersection {
    public:
        explicit Intersection(const string &name_);
        void addConnection(const string& road);

    private:
        string name;
        set<string> connected_segments;
};