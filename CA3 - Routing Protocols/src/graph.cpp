#include <algorithm>
#include <graph.hpp>
#include <string>
#include <utility>
#include <vector>

Graph::Node::Node(int id)
    : id_(id) {}

int Graph::Node::getId() const {
    return id_;
}

void Graph::Node::addNeighbor(Node* neighbor, int link_cost) {
    if (neighbor == this)
        ; // Throw exception

    auto new_link = std::pair<int, Node*>(link_cost, neighbor);
    auto found_upper_bound = std::upper_bound(
        neighbors_.begin(),
        neighbors_.end(),
        new_link,
        [](const std::pair<int, Node*> f, const std::pair<int, Node*> s) {
            return f.second->getId() < s.second->getId();
        });
    neighbors_.insert(found_upper_bound, new_link);
}

Graph::Node* Graph::Node::getNeighbor(int neighbor_id) {
    return findLink(neighbor_id).second;
}

int Graph::Node::getLinkCost(int neighbor_id) const {
    return findLink(neighbor_id).first;
}

int Graph::Node::getLinkCost(const Graph::Node* neighbor) const {
    return getLinkCost(neighbor->getId());
}

std::pair<int, Graph::Node*> Graph::Node::findLink(int neighbor_id) const {
    auto found_neighbor = std::find_if(
        neighbors_.begin(),
        neighbors_.end(),
        [neighbor_id](const std::pair<int, Node&> x) {
            return x.second.getId() == neighbor_id;
        });

    if (found_neighbor == neighbors_.end())
        ; // Throw link not found;
    return *found_neighbor;
}

Graph::Graph(int nodes_count) {
    for (int i = 1; i <= nodes_count; ++i)
        nodes_.push_back(new Node(i));
}

void Graph::addLink(int src, int dest, int cost) {
    Node* src_node = findNode(src);
    Node* dest_node = findNode(dest);
    src_node->addNeighbor(dest_node, cost);
}

std::string Graph::toString() const {}

Graph::Node* Graph::findNode(int node_id) const {
    auto found_node = std::find_if(
        nodes_.begin(),
        nodes_.end(),
        [node_id](const Node* x) {
            return x->getId() == node_id;
        });

    if (found_node == nodes_.end())
        ; // Throw node not found
    return *found_node;
}
