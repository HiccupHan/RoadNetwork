# RoadNetwork
 Modeling a road network, for Nissan Research Interview. Consists of four classes: RoadNetwork, RoadSegment, Intersection, and POI.
## RoadNetwork
 - The main class. Creates a road network consisting of road segments, intersections, and points of interests (POIs), represented as nodes.
### Constructor: 
    RoadNetwork(const string &name_): constructs a network with specified name (name_)
### Member Functions:
    void cleanSlate(): clears the network
    string getNetworkName(): returns the name of the network

    int addRoadSegmentStartEnd(const string& name, const string& start_road_segment, bool start_is_end, const string& end_road_segment, bool end_is_end, int dist, int num_lanes, bool is_oneway):
        Adds a road segment with specified connected roads for both the start and end of the new segment
        name: name of the new road
        start_road_segment: an existing road that is connected to the beginning of the new road
        start_is_end: boolean value to indicate if the start_road_segment is connected to the new road through its end (true) or start (false)
        end_road_segment: an existing road that is connected to the end of the new road
        end_is_end: boolean value to indicate if the end_road_segment is connected to the new road through its end (true) or start (false)
        dist: length of the road
        num_lanes: number of lanes the road has
        is_oneway: boolean value to indicate if the road is a one-way (true) or not
        return value: 1 if succeed, 0 if critical failure (parameter logically incorrect), 2 if road segment already exists

    int addRoadSegmentNoStart(const string& name, const string& end_road_segment, bool is_end, int dist, int num_lanes, bool is_oneway):
        Adds a road segment with a road connected to its end but not beginning
        Same format as addRoadSegmentStartEnd
        is_end: equivalent to end_is_end in addRoadSegmentStartEnd
    
    int addRoadSegmentNoEnd(const string& name, const string& start_road_segment, bool is_end, int dist, int num_lanes, bool is_oneway):
        Adds a road segment with a road connected to its beginning but not end
        Same format as addRoadSegmentStartEnd
        is_end: equivalent to start_is_end in addRoadSegmentStartEnd

    int addRoadSegment(const string& name, int dist, int num_lanes, bool is_oneway):
        Adds an isolated road segment
        Same format as addRoadSegmentStartEnd

    int addIntersection(const string& name): 
        Adds a named intersection
        Return value: 1 if suceed, 2 if intersection already exists

    int addPOI(const string& name, const string& road_loc, int distance):
        Adds a point of interest
        name: name of the POI
        road_loc: name of the road the POI is on
        distance: distance from the start of the road to the POI

    pair<int, vector<string>> shortestPath(const string& poi1, const string& poi2):
        Finds the shortest path:
        poi1: name of origin POI. poi2: name of destination POI.
        Return value: pair.first is -1 if one of the POIs doesn't exist, INT_MAX if destination is not reachable, or positive integer for the shortest distance
        pair.second will be empty if the operation fails, or will be a vector string containing the sequence of road names that makes up the shortest path starting
        from poi1 and ending with poi2
    
    int printPrettyShortestPath(const string& poi1, const string& poi2):
        Prints out the shortest path to standard output
        Same format as shortestPath, retun values equivalent to pair.first in shortestPath

## POI
### Constructor:
    POI(const string &name_, const string &road_): constructs a named (_name) POI located on a road with name (road_)
### Member Functions:
    string getName() const: returns the name of POI
    string getRoad() const: returns the name of the road POI is on

## RoadSegment
### Constructor:
    RoadSegment(const string &name_, const string &start_intersection_, const string &end_intersection_, int length_, int num_lanes_):
        Constructs a road segment with name (name_), length (length_), and number of lanes (num_lanes_)
        name: name of road
        start_intersection: name of the intersection in the beginning of the road
        end_intersection: name of the intersection in the end of the road
        length: length of the road
        num_lanes: number of lanes
### Member Functions:
    int getLength() const: returns length of the road
    int getPOIStart(const string& poi) const: returns the distance from start of the road to poi
    int getPOIEnd(const string& poi) const: returns the distance from the end of the raod to poi
    string getStartIntersection() const: returns the name of the intersection at the end of the road
    string getEndIntersection() const: returns the name of the intersection at the end of the road
    string getName() const: returns the name of the road
    void addPOI(const string& poi, int loc): adds a poi to the current road at distance from the start (loc)

## Intersection
### Constructor:
    Intersection(const string &name_): Constructs an intersection with specified name (name_)
### Member Functions:
    set<string> getConnectedRoads() const: returns a set of names of roads that are connected to the intersection
    string getName() const: returns the name of the intersection
    addConnection(const string& road): adds a new road that is connected to the current one
