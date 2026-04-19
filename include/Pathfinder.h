#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "MapGraph.h"
#include <vector>
#include <queue>
#include <unordered_map>
#include <limits>

class GraphPathfinder {
public:
    // Returns a list of Node IDs representing the path
    static std::vector<long long> FindPath(MapGraph& graph, long long startNodeId, long long targetNodeId);
};

#endif
