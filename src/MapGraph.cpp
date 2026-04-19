#include "MapGraph.h"
#include <algorithm>
#include <cmath>
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

MapGraph::MapGraph() {
    minLat = std::numeric_limits<double>::max();
    maxLat = std::numeric_limits<double>::lowest();
    minLon = std::numeric_limits<double>::max();
    maxLon = std::numeric_limits<double>::lowest();
}

void MapGraph::addNode(long long id, double lat, double lon, std::string name) {
    nodes[id] = MapNode(id, lat, lon, name);
    
    if (!name.empty()) {
        namedNodes.push_back(id);
    }
    
    // Update bounds
    if (lat < minLat) minLat = lat;
    if (lat > maxLat) maxLat = lat;
    if (lon < minLon) minLon = lon;
    if (lon > maxLon) maxLon = lon;
}

void MapGraph::addEdge(long long sourceId, long long targetId, double weightMultiplier) {
    if (nodes.find(sourceId) == nodes.end() || nodes.find(targetId) == nodes.end()) {
        return; // Skip if nodes don't exist
    }

    double weight = calculateDistance(nodes[sourceId].lat, nodes[sourceId].lon, 
                                      nodes[targetId].lat, nodes[targetId].lon);
    
    // Apply penalty/multiplier (e.g., residential roads "feel" longer)
    weight *= weightMultiplier;

    // Add edge in both directions (assuming roads are bidirectional for now, 
    // real OSM has 'oneway' tags but we'll simplify)
    adjacencyList[sourceId].push_back(MapEdge(targetId, weight));
    adjacencyList[targetId].push_back(MapEdge(sourceId, weight));
}

double MapGraph::calculateDistance(double lat1, double lon1, double lat2, double lon2) {
    // Haversine Formula
    double R = 6371000.0; // Earth radius in meters
    double dLat = (lat2 - lat1) * M_PI / 180.0;
    double dLon = (lon2 - lon1) * M_PI / 180.0;
    
    double a = sin(dLat / 2) * sin(dLat / 2) +
               cos(lat1 * M_PI / 180.0) * cos(lat2 * M_PI / 180.0) *
               sin(dLon / 2) * sin(dLon / 2);
    
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return R * c;
}

void MapGraph::projectToScreen(int screenWidth, int screenHeight, int padding) {
    if (nodes.empty()) return;

    // Simple linear projection
    // Invert Y because screen coordinates go down
    
    double latRange = maxLat - minLat;
    double lonRange = maxLon - minLon;
    
    if (latRange == 0) latRange = 1.0;
    if (lonRange == 0) lonRange = 1.0;

    // Maintain aspect ratio
    double mapAspectRatio = lonRange / latRange; // Not strictly accurate due to Mercator but close enough for local
    // Actually, one degree of lat is ~111km, one degree of lon is ~111km * cos(lat).
    // Let's correct the aspect ratio based on middle latitude
    double avgLatRad = ((minLat + maxLat) / 2.0) * M_PI / 180.0;
    double lonConversion = cos(avgLatRad); 
    
    double correctedLonRange = lonRange * lonConversion;
    
    int usableWidth = screenWidth - 2 * padding;
    int usableHeight = screenHeight - 2 * padding;
    
    double scaleX = usableWidth / correctedLonRange;
    double scaleY = usableHeight / latRange;
    double scale = std::min(scaleX, scaleY); // Uniform scale

    for (auto& pair : nodes) {
        MapNode& node = pair.second;
        
        // Normalize 0-1
        double nLat = (node.lat - minLat); // 0 at minLat
        double nLon = (node.lon - minLon) * lonConversion; // 0 at minLon
        
        node.screenX = padding + (int)(nLon * scale);
        // Invert Y: Map maxLat should be screen min Y (padding)
        node.screenY = usableHeight + padding - (int)(nLat * scale);
    }
}

long long MapGraph::getNearestNode(int screenX, int screenY) {
    long long nearestId = -1;
    double minDistSq = std::numeric_limits<double>::max();
    
    for (const auto& pair : nodes) {
        const MapNode& node = pair.second;
        double dx = node.screenX - screenX;
        double dy = node.screenY - screenY;
        double distSq = dx*dx + dy*dy;
        
        if (distSq < minDistSq) {
            minDistSq = distSq;
            nearestId = node.id;
        }
    }
    return nearestId;
}
