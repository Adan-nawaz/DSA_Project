#ifndef MAPGRAPH_H
#define MAPGRAPH_H

#include <vector>
#include <unordered_map>
#include <string>
#include <cmath>
#include <limits>

// Represents a node in the OSM graph (an intersection or point on a road)
struct MapNode {
    long long id;
    double lat;
    double lon;
    
    // For visualization logic
    int screenX;
    int screenY;
    std::string name; // Name of the location (e.g., "Jhelum")

    MapNode() : id(0), lat(0.0), lon(0.0), screenX(0), screenY(0), name("") {}
    MapNode(long long _id, double _lat, double _lon, std::string _name = "") 
        : id(_id), lat(_lat), lon(_lon), screenX(0), screenY(0), name(_name) {}
};

// Represents a connection between two nodes (a road segment)
struct MapEdge {
    long long targetNodeId;
    double weight; // Distance in meters or cost

    MapEdge(long long title, double w) : targetNodeId(title), weight(w) {}
};

class MapGraph {
public:
    // Stores all nodes by their OSM ID
    std::unordered_map<long long, MapNode> nodes;
    
    // Adjacency list: Node ID -> List of Edges
    std::unordered_map<long long, std::vector<MapEdge>> adjacencyList;

    // List of nodes that have names (for search/display)
    std::vector<long long> namedNodes;

    // Bounds of the map
    double minLat, maxLat, minLon, maxLon;

    MapGraph();

    void addNode(long long id, double lat, double lon, std::string name = "");
    void addEdge(long long sourceId, long long targetId, double weightMultiplier = 1.0);
    
    // Calculate distance between two coordinate points (Haversine formula)
    static double calculateDistance(double lat1, double lon1, double lat2, double lon2);

    // Project lat/lon to screen coordinates
    void projectToScreen(int screenWidth, int screenHeight, int padding);
    
    // Helper to get nearest node to a click
    long long getNearestNode(int screenX, int screenY);
};

#endif // MAPGRAPH_H
