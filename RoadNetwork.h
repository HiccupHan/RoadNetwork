#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include "RoadSegment.h"
#include "Intersection.h"
#include "POI.h"
using namespace std;

class RoadNetwork {
    public:
        explicit RoadNetwork(const string &name_);
        ~RoadNetwork();
        // clear the network
        void cleanSlate();
        string getNetworkName();

        /* add road segments */
        // adding road segment with specified start and end connections
        int addRoadSegmentStartEnd(const string& name, const string& start_road_segment, bool start_is_end, const string& end_road_segment, bool end_is_end, int dist, int num_lanes, bool is_oneway);
        // adding road segment with specified end connection
        int addRoadSegmentNoStart(const string& name, const string& end_road_segment, bool is_end, int dist, int num_lanes, bool is_oneway);
        // adding road segment with specified start connection
        int addRoadSegmentNoEnd(const string& name, const string& start_road_segment, bool is_end, int dist, int num_lanes, bool is_oneway);
        // adding an isolated road segment
        int addRoadSegment(const string& name, int dist, int num_lanes, bool is_oneway);
        // adding a named intersection
        int addIntersection(const string& name);
        int addPOI(const string& name, const string& road_loc, int distance);

        /* shortest path implementations */
        pair<int, vector<string>> shortestPath(const string& poi1, const string& poi2);
        int printPrettyShortestPath(const string& poi1, const string& poi2);

    private:
        string name;
        unordered_map<string, Intersection*> intersections;
        unordered_map<string, RoadSegment*> road_segments;
        unordered_map<string, POI*> POIs;
        /* helper functions */
        void addSegment(const string& name, const string& start_intersection, const string& end_intersection, int dist, int num_lanes, bool is_oneway);
        static string findMin(const unordered_map<string, tuple<int, string, string>>& reachable_intersections);
        static vector<string> reconstructPath(const unordered_map<string, pair<string, string>>& visited, const string& poi1, const string& poi2);
        void processReachableIntersects(unordered_map<string, pair<string, string>>& visited, unordered_map<string, tuple<int, string, string>>& reachable_intersections, const string& next_intersect, const string& curr_intersect_name, const string& road_name, int curr_dist);
};