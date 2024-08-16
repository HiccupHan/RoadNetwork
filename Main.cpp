#include <string>
#include <iostream>
#include "RoadNetwork.h"
using namespace std;

int main() {
    RoadNetwork r = RoadNetwork("test");
    r.addRoadSegment("N4", 4, 4);
    r.addRoadSegmentNoEnd("N1","N4",true, 4, 4);
    r.addRoadSegmentNoEnd("N3", "N1", false, 2, 4);
    r.addRoadSegmentNoEnd("N6", "N3", true, 1, 4);
    r.addRoadSegmentStartEnd("N5", "N4", true, "N6", true, 8, 4);
    r.addRoadSegmentNoEnd("N2", "N4", true, 2, 4);
    r.addRoadSegmentStartEnd("N7", "N1", true, "N2", true, 3, 4);
    r.addRoadSegmentNoEnd("N8", "N2", true, 2, 4);
    r.addPOI("A", "N4", 1);
    r.addPOI("B", "N4", 3);
    r.addPOI("C", "N5", 7);
    r.addPOI("D", "N8", 1);
    r.printPrettyShortestPath("A", "D");
    return 1;
}