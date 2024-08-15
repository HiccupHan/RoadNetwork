#pragma once

#include <unordered_map>
#include "RoadSegment.h"
#include "Intersection.h"
#include "POI.h"
using namespace std;

class RoadNetwork {
    public:
        explicit RoadNetwork(string name_);
        ~RoadNetwork();
        void cleanSlate();
        int addRoadSegmentStartEnd(const string& name, const string& start_road_segment, const string& end_road_segment, int dist, int num_lanes);
        int addRoadSegmentNoStart(const string& name, const string& end_road_segment, int dist, int num_lanes);
        int addRoadSegmentNoEnd(const string& name, const string& start_road_segment, int dist, int num_lanes);
        int addRoadSegment(const string& name, int dist, int num_lanes);
        int addIntersection(const string& name, vector<RoadSegment> &connected_roads);
        int addPOI(const string& name, const string& road_loc, int distance);

        pair<int, vector<string>> shortestPath(string poi1, string poi2);
        RoadSegment* getRoad(const string& name);
    private:
        string name;
        unordered_map<string, Intersection*> intersections;
        unordered_map<string, RoadSegment*> road_segments;
        unordered_map<string, POI*> POIs;
        void addSegment(const string& name, const string& start_intersection, const string& end_intersection, int dist, int num_lanes);
};