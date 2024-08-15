#pragma once

#include <vector>
#include <string>
using namespace std;

class POI {
    public:
        POI(const string &name_, const string &road_);
        string getName();
        string getRoad();
    private:
        string road;
        string name;
};
