#pragma once

#include <string>
using namespace std;

class POI {
    public:
        POI(const string &name_, const string &road_);
        string getName() const;
        // get the name of the road POI is on
        string getRoad() const;

    private:
        string road;
        string name;
};
