#include "Pathfinder.h"
#include <algorithm>
#include <queue>
#include <set>

// Helper struct for Priority Queue
struct PQNode {
    long long id;
    double f_score;

    bool operator>(const PQNode& other) const {
        return f_score > other.f_score; // Min-heap based on f_score
    }
};

std::vector<long long> GraphPathfinder::FindPath(MapGraph& graph, long long startNodeId, long long targetNodeId) {
    if (graph.nodes.find(startNodeId) == graph.nodes.end() || 
        graph.nodes.find(targetNodeId) == graph.nodes.end()) {
        return {}; // Invalid nodes
    }

    // Standard A* Implementation
    
    // Open Set (Min-Heap)
    std::priority_queue<PQNode, std::vector<PQNode>, std::greater<PQNode>> openSet;
    openSet.push({startNodeId, 0.0});

    // Came From (for path reconstruction)
    std::unordered_map<long long, long long> cameFrom;

    // G Score (Cost from start)
    std::unordered_map<long long, double> gScore;
    gScore[startNodeId] = 0.0;

    // F Score (G + H)
    // We don't strictly need to store F scores in a map if we push them to PQ, 
    // but tracking "visited" or "closed" is helpful. 
    // Using a map to track if we found a better path.
    
    std::set<long long> inOpenSet; // Helper to check existence efficiently
    inOpenSet.insert(startNodeId);

    while (!openSet.empty()) {
        PQNode current = openSet.top();
        openSet.pop();
        
        long long currentId = current.id;
        inOpenSet.erase(currentId);

        if (currentId == targetNodeId) {
            // Reconstruct path
            std::vector<long long> path;
            long long temp = currentId;
            while (cameFrom.find(temp) != cameFrom.end()) {
                path.push_back(temp);
                temp = cameFrom[temp];
            }
            path.push_back(startNodeId);
            std::reverse(path.begin(), path.end());
            return path;
        }

        // For each neighbor
        if (graph.adjacencyList.find(currentId) != graph.adjacencyList.end()) {
            for (const auto& edge : graph.adjacencyList[currentId]) {
                long long neighborId = edge.targetNodeId;
                double tentative_gScore = gScore[currentId] + edge.weight;

                if (gScore.find(neighborId) == gScore.end() || tentative_gScore < gScore[neighborId]) {
                    // Found a better path
                    cameFrom[neighborId] = currentId;
                    gScore[neighborId] = tentative_gScore;
                    
                    double h = MapGraph::calculateDistance(
                        graph.nodes[neighborId].lat, graph.nodes[neighborId].lon,
                        graph.nodes[targetNodeId].lat, graph.nodes[targetNodeId].lon
                    );
                    
                    double f = tentative_gScore + h;
                    
                    openSet.push({neighborId, f});
                    inOpenSet.insert(neighborId);
                }
            }
        }
    }

    return {}; // No path found
}
