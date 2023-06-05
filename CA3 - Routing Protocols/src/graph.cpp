#include <graph.hpp>

Graph::Graph(int nodes_count)
    : adj_vec_(nodes_count, std::vector<Graph::Link>(nodes_count)) {}

void Graph::addLink(int src, int dest, int cost) {
    adj_vec_[src - 1][dest - 1].cost_ = cost;
    adj_vec_[src - 1][dest - 1].is_connected_ = true;

    adj_vec_[dest - 1][src - 1].cost_ = cost;
    adj_vec_[dest - 1][src - 1].is_connected_ = true;
}

void Graph::changeLinkCost(int src, int dest, int new_cost) {
    adj_vec_[src - 1][dest - 1].cost_ = new_cost;
    adj_vec_[dest - 1][src - 1].cost_ = new_cost;
}

int Graph::getCost(int src, int dest) const {
    return adj_vec_[src - 1][dest - 1].cost_;
}

std::string Graph::toString() const {
}
