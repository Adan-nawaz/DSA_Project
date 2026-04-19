#include "OSMParser.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

// ---------------- HELPER FUNCTIONS ----------------

// Extract attribute value from XML line
// Helper to extract attribute value
// e.g., getAttr(line, "lat") from " <node ... lat=\"12.34\" ... > "
std::string getAttr(const std::string& line, const std::string& key) {
    std::string search = key + "=\"";
    size_t pos = line.find(search);
    if (pos == std::string::npos) {
        search = key + "='";
        pos = line.find(search);
        if (pos == std::string::npos) return "";
    }
    
    pos += search.length();
    size_t endPos = line.find("\"", pos);
    if (endPos == std::string::npos) endPos = line.find("'", pos);
    
    if (endPos != std::string::npos) {
        return line.substr(pos, endPos - pos);
    }
    return "";
}

// Helper: Strict English/ASCII filter
// Only allows: A-Z, a-z, 0-9, space, and basic punctuation (., -)
bool isReadableReference(const std::string& s) {
    if (s.empty()) return false;
    int letterCount = 0;
    for (char c : s) {
        // Allow basic ASCII printables only
        if (c < 32 || c > 126) return false;
        if (isalpha(c)) letterCount++;
    }
    // Must contain at least one actual letter to be a valid name
    return letterCount > 0;
}

// ---------------- PARSER FUNCTION ----------------

bool OSMParser::parseOSMFile(const std::string& filename, MapGraph& graph) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open OSM file: " << filename << std::endl;
        return false;
    }

    std::string line;
    bool inNode = false;
    bool inWay = false;

    long long currentNodeId = 0;
    double currentLat = 0;
    double currentLon = 0;
    std::vector<long long> wayNodes;
    double currentPenalty = 1.0;

    while (std::getline(file, line)) {

        // ---------------- NODE PARSING ----------------
        if (line.find("<node") != std::string::npos) {
            std::string idStr = getAttr(line, "id");
            std::string latStr = getAttr(line, "lat");
            std::string lonStr = getAttr(line, "lon");

            if (!idStr.empty() && !latStr.empty() && !lonStr.empty()) {
                currentNodeId = std::stoll(idStr);
                currentLat = std::stod(latStr);
                currentLon = std::stod(lonStr);

                // Add node immediately (empty name)
                graph.addNode(currentNodeId, currentLat, currentLon, "");
                inNode = true;
            }
        }

        if (inNode) {
            if (line.find("<tag") != std::string::npos) {
                std::string k = getAttr(line, "k");
                std::string v = getAttr(line, "v");

                if (k == "name") {
                    // Update node name (Unicode-friendly)
                    if (isReadableReference(v)) {
                         graph.nodes[currentNodeId].name = v;
                         graph.namedNodes.push_back(currentNodeId);
                    }
                }
            }

            if (line.find("</node>") != std::string::npos) {
                inNode = false;
            }
        }

        // ---------------- WAY PARSING ----------------
        if (line.find("<way") != std::string::npos) {
            inWay = true;
            wayNodes.clear();
            currentPenalty = 1.0;
        }

        if (inWay) {
            // Parse node references
            if (line.find("<nd") != std::string::npos) {
                std::string refStr = getAttr(line, "ref");
                if (!refStr.empty()) {
                    wayNodes.push_back(std::stoll(refStr));
                }
            }

            // Parse highway type
            if (line.find("<tag") != std::string::npos) {
                std::string k = getAttr(line, "k");
                std::string v = getAttr(line, "v");

                if (k == "highway") {
                    if (v == "motorway" || v == "motorway_link") currentPenalty = 1.0;
                    else if (v == "trunk" || v == "trunk_link") currentPenalty = 1.1;
                    else if (v == "primary" || v == "primary_link") currentPenalty = 1.2;
                    else if (v == "secondary" || v == "secondary_link") currentPenalty = 1.3;
                    else if (v == "tertiary" || v == "tertiary_link") currentPenalty = 1.5;
                    else if (v == "residential") currentPenalty = 2.0;
                    else if (v == "service" || v == "unclassified") currentPenalty = 3.0;
                }

                // Optional: parse street/road name for future labels
                if (k == "name") {
                    // Could store edge name if you want
                    // TODO: graph.edges.back().name = v;
                }
            }

            // End of way
            if (line.find("</way>") != std::string::npos) {
                inWay = false;

                if (wayNodes.size() > 1) {
                    for (size_t i = 0; i < wayNodes.size() - 1; ++i) {
                        graph.addEdge(wayNodes[i], wayNodes[i + 1], currentPenalty);
                    }
                }
            }
        }
    }

    file.close();
    std::cout << "OSM parsing complete. Nodes: " << graph.nodes.size()
              << ", Adjacency entries: " << graph.adjacencyList.size() << std::endl;
    return true;
}
