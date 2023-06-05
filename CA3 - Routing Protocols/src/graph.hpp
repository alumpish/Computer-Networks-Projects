#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <utility>
#include <vector>

class Graph {
public:
    class Node {
    public:
        Node(int id);
        int getId() const;
        void addNeighbor(Node* neighbor, int link_cost);
        Node* getNeighbor(int neighbor_id);
        int getLinkCost(int neighbor_id) const;
        int getLinkCost(const Node* neighbor) const;

    private:
        int id_;
        std::vector<std::pair<int, Node*>> neighbors_;

        std::pair<int, Node*> findLink(int neighbor_id) const;
    };

    // Node ids are 1 based
    Graph(int nodes_count);
    void addLink(int src, int dest, int cost);
    std::string toString() const;

private:
    std::vector<Node*> nodes_;

    Node* findNode(int node_id) const;
};

#endif