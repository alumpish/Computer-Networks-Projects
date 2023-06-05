#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <vector>

#include "graph.hpp"

class Network {
public:
    Network(Graph& graph);
    int minDistance(std::vector<int> dist, std::vector<bool> visited);
    void printDist(std::vector<int> dist);
    void printPath(std::vector<int> parent, int j);
    void printNode(int source, std::vector<int> dist, std::vector<int> parent, int n);
    void lsrp();
    void lsrp(int source);
    void dvrp();
    void dvrp(int source);

private:
    Graph& graph_;
};

#endif
