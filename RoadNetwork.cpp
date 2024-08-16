#include "RoadNetwork.h"

#include <algorithm>
#include <iostream>
#include <ostream>
#include <vector>
#include "RoadSegment.h"
#include "Intersection.h"
#include <cstdio>
#include <queue>
using namespace std;

RoadNetwork::RoadNetwork(const string &name_) {
    name = name_;
}
RoadNetwork::~RoadNetwork() {
    cleanSlate();
}
string RoadNetwork::getNetworkName() {
    return name;
}
void RoadNetwork::cleanSlate() {
    for (auto & road_segment : road_segments) {
        delete road_segment.second;
    }
    road_segments.clear();
    for (auto & POI : POIs) {
        delete POI.second;
    }
    POIs.clear();
    for (auto & intersection : intersections) {
        delete intersection.second;
    }
    intersections.clear();
}

int RoadNetwork::addRoadSegmentStartEnd(const string& name, const string& start_road_segment, bool start_is_end, const string& end_road_segment, bool end_is_end, const int dist, const int num_lanes, bool is_oneway) {
    /* error check */
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

    // determine the start intersection and end intersection's name
    const string start_inter = start_is_end ? road_segments[start_road_segment]->getEndIntersection() : road_segments[start_road_segment]->getStartIntersection();
    const string end_inter = end_is_end ? road_segments[end_road_segment]->getEndIntersection() : road_segments[end_road_segment]->getStartIntersection();
    addSegment(name, start_inter, end_inter, dist, num_lanes, is_oneway);
    return 1;
}

int RoadNetwork::addRoadSegmentNoStart(const string &name, const string &end_road_segment, bool is_end, int dist, int num_lanes, bool is_oneway) {
    /* error check */
    if(dist <= 0 || num_lanes <= 0 || road_segments.find(end_road_segment) == road_segments.end()) {
        fprintf(stderr, "Failed: incorrect parameters for road segments\n");
        return 0;
    }
    if (road_segments.find(name) != road_segments.end()) {
        fprintf(stderr, "Warning: road segment already exists\n");
        return 2;
    }

    const string start_inter = to_string(intersections.size());
    const string end_inter = is_end ? road_segments[end_road_segment]->getEndIntersection() : road_segments[end_road_segment]->getStartIntersection();
    // create a new intersection since none was specified
    intersections[start_inter] = new Intersection(start_inter);
    addSegment(name, start_inter, end_inter, dist, num_lanes, is_oneway);
    return 1;
}

int RoadNetwork::addRoadSegmentNoEnd(const string &name, const string &start_road_segment, bool is_end, int dist, int num_lanes, bool is_oneway) {
    /* error check */
    if(dist <= 0 || num_lanes <= 0 || road_segments.find(start_road_segment) == road_segments.end()) {
        fprintf(stderr, "Failed: incorrect parameters for road segments\n");
        return 0;
    }
    if (road_segments.find(name) != road_segments.end()) {
        fprintf(stderr, "Warning: road segment already exists\n");
        return 2;
    }

    const string start_inter = is_end ? road_segments[start_road_segment]->getEndIntersection() : road_segments[start_road_segment]->getStartIntersection();
    const string end_inter = to_string(intersections.size());
    intersections[end_inter] = new Intersection(end_inter);
    addSegment(name, start_inter, end_inter, dist, num_lanes, is_oneway);
    return 1;
}

int RoadNetwork::addRoadSegment(const string &name, const int dist, const int num_lanes, bool is_oneway) {
    /* error check */
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
    addSegment(name, start_inter, end_inter, dist, num_lanes, is_oneway);
    return 1;
}

void RoadNetwork::addSegment(const string& name, const string& start_intersection, const string& end_intersection, int dist, int num_lanes, bool is_oneway) {
    intersections[start_intersection]->addConnection(name);
    // if the road is a one way, you are not allowed to travel backwards
    if(!is_oneway)
        intersections[end_intersection]->addConnection(name);
    road_segments[name] = new RoadSegment(name, start_intersection, end_intersection, dist, num_lanes);
}

int RoadNetwork::addIntersection(const string& name) {
    /* error check */
    if (intersections.find(name) != intersections.end()) {
        fprintf(stderr, "Warning: intersection already exists\n");
        return 2;
    }
    intersections[name] = new Intersection(name);
    return 1;
}

int RoadNetwork::addPOI(const string& name, const string& road_loc, const int distance) {
    /* error check */
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

/* helper function: finds the next closest reachable intersection */
string RoadNetwork::findMin(const unordered_map<string, tuple<int, string, string>>& reachable_intersections) {
    string min_intersect;
    int min_dist = INT_MAX;
    for (const auto& intersect : reachable_intersections) {
        if(get<0>(intersect.second) < min_dist) {
            min_dist = get<0>(intersect.second);
            min_intersect = intersect.first;
        }
    }
    return min_intersect;
}

/* helper function: builds the shortest path based on visited intersection information */
vector<string> RoadNetwork::reconstructPath(const unordered_map<string, pair<string, string>> &visited, const string& poi1, const string& poi2) {
    vector<string> path;
    string curr_intersection = poi2;
    while (curr_intersection != poi1) {
        path.push_back(visited.find(curr_intersection)->second.second);
        curr_intersection = visited.find(curr_intersection)->second.first;
    }
    reverse(path.begin(), path.end());
    return path;
}

/* helper function: adds/updates reachable intersection on the current road */
void RoadNetwork::processReachableIntersects(unordered_map<string, pair<string, string> > &visited, unordered_map<string, tuple<int, string, string> > &reachable_intersections, const string& next_intersect, const string &curr_intersect_name, const string &road_name, int curr_dist) {
    if(visited.find(next_intersect) == visited.end()) {
        if(reachable_intersections.find(next_intersect) == reachable_intersections.end()) {
            reachable_intersections[next_intersect] = make_tuple(road_segments[road_name]->getLength() + curr_dist, curr_intersect_name, road_name);
        }
        else {
            if(get<0>(reachable_intersections[next_intersect]) > road_segments[road_name]->getLength() + curr_dist) {
                get<0>(reachable_intersections[next_intersect]) = road_segments[road_name]->getLength() + curr_dist;
                get<1>(reachable_intersections[next_intersect]) = curr_intersect_name;
                get<2>(reachable_intersections[next_intersect]) = road_name;
            }
        }
    }
}

/* Dijkstra's algo with modification: find the shortest path to both the start and end intersection of the road the destination is on */
pair<int, vector<string> > RoadNetwork::shortestPath(const string& poi1, const string& poi2) {
    int shortest_dist = 0;
    vector<string> shortest_path;

    /* POIs must exist */
    if (POIs.find(poi1) == POIs.end() || POIs.find(poi2) == POIs.end()) {
        fprintf(stderr, "Failed: POIs do not exist\n");
        return make_pair(-1, vector<string>());
    }

    /* POI objects */
    POI *start_loc = POIs[poi1];
    POI *end_loc = POIs[poi2];
    /* roads POIs are on */
    string start_road_name = start_loc->getRoad();
    string end_road_name = end_loc->getRoad();
    RoadSegment *start_road_segment = road_segments[start_road_name];
    RoadSegment *end_road_segment = road_segments[end_road_name];
    // the start intersection of the road destination is on
    string targetIntersection1 = end_road_segment->getStartIntersection();
    // the end intersection of the road destination is on
    string targetIntersection2 = end_road_segment->getEndIntersection();

    // key: the current intersection. value: the intersection we came from and current road name
    unordered_map<string, pair<string, string>> visited;
    // first int: distance to the start intersection on the road of destination. second int: distance to the end intersection ~
    pair<int, int> dist_to_intersects = make_pair(-1, -1);

    /* if POIs are on the same road */
    if(start_road_name == end_road_name) {
        shortest_path.push_back(start_road_name);
        shortest_dist = abs(road_segments[start_road_name]->getPOIStart(poi1) - road_segments[end_road_name]->getPOIStart(poi2));
        return make_pair(shortest_dist, shortest_path);
    }

    /* If the road the destination is on has no other roads connecting to it from the start or end, the corresponding side can be ignored */
    if(intersections[targetIntersection1]->getConnectedRoads().size() <= 1) {
        dist_to_intersects.first = INT_MAX;
        visited[targetIntersection1] = make_pair(poi1, start_road_name);
    }
    if(intersections[targetIntersection2]->getConnectedRoads().size() <= 1) {
        dist_to_intersects.second = INT_MAX;
        visited[targetIntersection2] = make_pair(poi1, start_road_name);
    }
    /* error check: if destination is on an isolated road, it is unreachable */
    if(dist_to_intersects.first == INT_MAX && dist_to_intersects.second == INT_MAX) {
        fprintf(stderr, "Failed: Target POI unreachable\n");
        return make_pair(-1, vector<string>());
    }

    // key: intersection name. value: distance from origin, intersection we came from, current road name
    unordered_map<string, tuple<int, string, string>> reachable_intersections;
    reachable_intersections[start_road_segment->getStartIntersection()] = make_tuple(start_road_segment->getPOIStart(poi1), poi1, start_road_name);
    reachable_intersections[start_road_segment->getEndIntersection()] = make_tuple(start_road_segment->getPOIEnd(poi1), poi1, start_road_name);

    /* Dijkstra's algo */
    while(!reachable_intersections.empty()) {
        auto curr_intersect_name = findMin(reachable_intersections);
        tuple<int, string, string> intersect_info = reachable_intersections[curr_intersect_name];
        string road_to_curr_intersect = get<2>(intersect_info);
        string prev_intersect = get<1>(intersect_info);
        int curr_dist = get<0>(intersect_info);

        visited[curr_intersect_name] = make_pair(prev_intersect, road_to_curr_intersect);
        if (curr_intersect_name == targetIntersection1)
            dist_to_intersects.first = curr_dist;
        else if (curr_intersect_name == targetIntersection2)
            dist_to_intersects.second = curr_dist;
        /* we found both shortest paths */
        if (dist_to_intersects.first != -1 &&  dist_to_intersects.second != -1)
            break;

        for (const auto& road_name : intersections[curr_intersect_name]->getConnectedRoads()) {
            string road_s_intersect = road_segments[road_name]->getStartIntersection();
            string road_e_intersect = road_segments[road_name]->getEndIntersection();
            processReachableIntersects(visited, reachable_intersections, road_s_intersect, curr_intersect_name, road_name, curr_dist);
            processReachableIntersects(visited, reachable_intersections, road_e_intersect, curr_intersect_name, road_name, curr_dist);
        }
        reachable_intersections.erase(curr_intersect_name);
    }

    if (dist_to_intersects.first == INT_MAX && dist_to_intersects.second == INT_MAX) {
        fprintf(stderr, "Failed: Target POI unreachable\n");
        return make_pair(-1, vector<string>());
    }

    /* find out which intersection we should come from */
    if (dist_to_intersects.first == INT_MAX) {
        shortest_dist = dist_to_intersects.second + end_road_segment->getPOIEnd(poi2);
        visited[poi2] = make_pair(end_road_segment->getEndIntersection(), end_road_name);
    }
    else if (dist_to_intersects.second == INT_MAX) {
        shortest_dist = dist_to_intersects.first + end_road_segment->getPOIStart(poi2);
        visited[poi2] = make_pair(end_road_segment->getStartIntersection(), end_road_name);
    }
    else {
        int from_start = dist_to_intersects.first + end_road_segment->getPOIStart(poi2);
        int from_end = dist_to_intersects.second + end_road_segment->getPOIEnd(poi2);
        shortest_dist = min(from_start, from_end);
        visited[poi2] = from_start < from_end ? make_pair(end_road_segment->getStartIntersection(), end_road_name) : make_pair(end_road_segment->getEndIntersection(), end_road_name);
    }

    shortest_path = reconstructPath(visited, poi1, poi2);
    return make_pair(shortest_dist, shortest_path);
}

int RoadNetwork::printPrettyShortestPath(const string &poi1, const string &poi2) {
    pair<int, vector<string>> shortest_path_info = shortestPath(poi1, poi2);
    int shortest_dist = shortest_path_info.first;
    vector<string> path = shortest_path_info.second;
    if (shortest_dist == INT_MAX || shortest_dist < 0)
        return 0;
    printf("Shortest Path From %s to %s:\n", poi1.c_str(), poi2.c_str());
    printf("    Shortest Path Length: %i\n", shortest_dist);
    printf("    Shortest Path: %s-", poi1.c_str());
    for (const auto& road_name : path) {
        printf("%s-", road_name.c_str());
    }
    printf("%s\n", poi2.c_str());
    return 1;
}