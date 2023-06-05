#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <utility>
#include <vector>

class Graph {
public:
    struct Link {
        int cost_ = -1;
        bool is_connected_ = false;
    };

    // Node ids are 1 based
    Graph(int nodes_count);
    void addLink(int src, int dest, int cost);
    void changeLinkCost(int src, int dest, int new_cost);
    int getCost(int src, int dest) const;
    int getNodesCount() const;
    
    std::string toString() const;
    std::vector<int> shortestPath(int src, int dest) const;

private:
    std::vector<std::vector<Link>> adj_vec_;
};

#endif