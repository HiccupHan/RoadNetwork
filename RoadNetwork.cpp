#include "RoadNetwork.h"

#include <iostream>
#include <ostream>
#include <vector>
#include "RoadSegment.h"
#include "Intersection.h"
#include <cstdio>
using namespace std;

RoadNetwork::RoadNetwork(string name_) {
    name = name_;
}
RoadNetwork::~RoadNetwork() {
    cleanSlate();
}

int RoadNetwork::addRoadSegmentStartEnd(const string& name, const string& start_road_segment, const string& end_road_segment, const int dist, const int num_lanes) {
    if (start_road_segment.empty() || end_road_segment.empty() ||
        dist <= 0 || num_lanes <= 0 ||
        road_segments.find(start_road_segment) == road_segments.end() ||
        road_segments.find(end_road_segment) == road_segments.end()) {
        fprintf(stderr, "Failed: incorrect parameters for road segments\n");
        return 0;
        }
    if (road_segments.find(name) != road_segments.end()) {
        fprintf(stderr, "Warning: road segment already exists\n");
        return 2;
    }
    const string start_inter = road_segments[start_road_segment]->getStartIntersection();
    const string end_inter = road_segments[end_road_segment]->getEndIntersection();
    addSegment(name, start_inter, end_inter, dist, num_lanes);
    return 1;
}

int RoadNetwork::addRoadSegmentNoStart(const string &name, const string &end_road_segment, int dist, int num_lanes) {
    if(dist <= 0 || num_lanes <= 0 || road_segments.find(end_road_segment) == road_segments.end()) {
        fprintf(stderr, "Failed: incorrect parameters for road segments\n");
        return 0;
    }
    if (road_segments.find(name) != road_segments.end()) {
        fprintf(stderr, "Warning: road segment already exists\n");
        return 2;
    }
    const string start_inter = to_string(intersections.size());
    const string end_inter = road_segments[end_road_segment]->getEndIntersection();
    intersections[start_inter] = new Intersection(start_inter);
    addSegment(name, start_inter, end_inter, dist, num_lanes);
    return 1;
}

int RoadNetwork::addRoadSegmentNoEnd(const string &name, const string &start_road_segment, int dist, int num_lanes) {
    if(dist <= 0 || num_lanes <= 0 || road_segments.find(start_road_segment) == road_segments.end()) {
        fprintf(stderr, "Failed: incorrect parameters for road segments\n");
        return 0;
    }
    if (road_segments.find(name) != road_segments.end()) {
        fprintf(stderr, "Warning: road segment already exists\n");
        return 2;
    }
    const string start_inter = road_segments[start_road_segment]->getStartIntersection();
    const string end_inter = to_string(intersections.size());
    intersections[end_inter] = new Intersection(end_inter);
    addSegment(name, start_inter, end_inter, dist, num_lanes);
    return 1;
}

int RoadNetwork::addRoadSegment(const string &name, int dist, int num_lanes) {
    if (dist <= 0 || num_lanes <= 0) {
        fprintf(stderr, "Failed: incorrect parameters for road segments\n");
        return 0;
    }
    if (road_segments.find(name) != road_segments.end()) {
        fprintf(stderr, "Warning: road segment already exists\n");
        return 2;
    }
    const string start_inter = to_string(intersections.size());
    const string end_inter = to_string(intersections.size() + 1);
    intersections[start_inter] = new Intersection(start_inter);
    intersections[end_inter] = new Intersection(end_inter);
    addSegment(name, start_inter, end_inter, dist, num_lanes);
    return 1;
}

void RoadNetwork::addSegment(const string& name, const string& start_intersection, const string& end_intersection, int dist, int num_lanes) {
    intersections[start_intersection]->addConnection(name);
    intersections[end_intersection]->addConnection(name);
    road_segments[name] = new RoadSegment(name, start_intersection, end_intersection, dist, num_lanes);
}


int RoadNetwork::addIntersection(const string& name, vector<RoadSegment> &connected_roads) {
    if (intersections.find(name) != intersections.end()) {
        fprintf(stderr, "Warning: intersection already exists\n");
        return 2;
    }
    intersections[name] = new Intersection(name);
    return 1;
}

int RoadNetwork::addPOI(const string& name, const string& road_loc, const int distance) {
    if (POIs.find(name) != POIs.end()) {
        fprintf(stderr, "Warning: POI already exists\n");
        return 2;
    }
    if (road_segments.find(road_loc) == road_segments.end() || distance > road_segments[road_loc]->getLength()) {
        fprintf(stderr, "Failed: incorrect parameters for POI\n");
        return 0;
    }
    POIs[name] = new POI(name, road_loc);
    road_segments[road_loc]->addPOI(name, distance);
    return 1;
}

RoadSegment *RoadNetwork::getRoad(const string& name) {
    if (road_segments.find(name) == road_segments.end()) {
        fprintf(stderr, "Failed: road segment does not exist\n");
        return nullptr;
    }
    return road_segments[name];
}

void RoadNetwork::cleanSlate() {
    for (auto it = road_segments.begin(); it != road_segments.end(); it++) {
        delete it->second;
    }
    road_segments.clear();
    for (auto it = POIs.begin(); it != POIs.end(); it++) {
        delete it->second;
    }
    POIs.clear();
    for (auto it = intersections.begin(); it != intersections.end(); it++) {
        delete it->second;
    }
    intersections.clear();
}

pair<int, vector<string> > RoadNetwork::shortestPath(string poi1, string poi2) {
    int shortest_dist = 0;
    vector<string> shortest_path;

    if (POIs.find(poi1) == POIs.end() || POIs.find(poi2) == POIs.end()) {
        fprintf(stderr, "Failed: POIs do not exist\n");
        return make_pair(-1, vector<string>());
    }

    POI *start_loc = POIs[poi1];
    POI *end_loc = POIs[poi2];
    string start_road_name = start_loc->getRoad();
    string end_road_name = end_loc->getRoad();

    if(start_road_name == end_road_name) {
        shortest_path.push_back(start_road_name);
        shortest_dist = abs(road_segments[start_road_name]->getPOIStart(poi1) - road_segments[end_road_name]->getPOIStart(poi2));
        return make_pair(shortest_dist, shortest_path);
    }

    RoadSegment *start_road_segment = road_segments[start_road_name];
    RoadSegment *end_road_segment = road_segments[end_road_name];

    set<string> visited;
    // auto cmp = [](const pair<int, string>& a, const pair<int, string>& b) { return a.first < b.first;};
    // set<pair<int, string>, decltype(cmp)> reachable_roads;
    // for(auto road : intersections[start_road_segment->getStartIntersection()])
    visited.insert(start_road_name);



    cout<<"start_road_name: "<<start_road_name<<endl;
    cout<<"end_road_name: "<<end_road_name<<endl;

    return make_pair(shortest_dist, shortest_path);
}


