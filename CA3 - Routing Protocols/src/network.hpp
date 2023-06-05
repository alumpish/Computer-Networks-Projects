#include "graph.hpp"

class Network {
public:
    Network(Graph& graph);
    void lsrp();
    void lsrp(int source);
    int minDistance(std::vector<int> dist, std::vector<bool> visited);
    void printPath(std::vector<int> parent, int j);
    void dvrp();
    void dvrp(int source);

private:
    Graph& graph_;
};