#pragma once
#include <set>
#include <string>
using namespace std;

class Intersection {
    public:
        explicit Intersection(const string &name_);
        set<string> getConnectedRoads() const;
        string getName() const;

        void addConnection(const string& road);

    private:
        string name;
        set<string> connected_segments;
};