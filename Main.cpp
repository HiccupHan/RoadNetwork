#include <string>
#include <iostream>
#include "RoadNetwork.h"
using namespace std;

int main() {
    RoadNetwork r = RoadNetwork("test");
    r.addRoadSegment("N1", 4, 4);
    r.addRoadSegmentNoEnd("N2", "N1", 2, 4);
    r.addRoadSegmentStartEnd("N3", "N1", "N2", 2, 4);
    r.addPOI("A", "N1", 1);
    r.addPOI("B", "N1", 4);
    r.shortestPath("A", "B");
    RoadSegment* road = r.getRoad("N1");
    cout<<road->getStartIntersection()<<endl;
    cout<<road->getEndIntersection()<<endl;
    cout<<road->getPOIStart("A")<<endl;
    cout<<road->getPOIEnd("A")<<endl;
    return 1;
}