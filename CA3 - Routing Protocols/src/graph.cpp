#include <graph.hpp>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

Graph::Graph() {}

void Graph::setNodesCount(int nodes_count) {
    adj_vec_ = std::vector<std::vector<Link>>(
        nodes_count, std::vector<Link>(nodes_count));
}

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

void Graph::removeLink(int src, int dest) {
    adj_vec_[src - 1][dest - 1].cost_ = -1;
    adj_vec_[dest - 1][src - 1].cost_ = -1;
}

int Graph::getCost(int src, int dest) const {
    return adj_vec_[src - 1][dest - 1].cost_;
}

std::string Graph::toString() const {
    std::ostringstream output_str;
    output_str << std::setw(5) << " ";
    output_str << "|";
    for (int i = 1; i <= adj_vec_.size(); ++i)
        output_str << std::setw(5) << i;
    output_str << std::endl;
    for (int i = 1; i <= adj_vec_.size(); ++i) {
        output_str << std::setw(5) << i;
        output_str << "|";
        for (int j = 1; j <= adj_vec_.size(); ++j) {
            if (i == j) {
                output_str << std::setw(5) << 0;
                continue;
            }
            output_str << std::setw(5) << adj_vec_[i - 1][j - 1].cost_;
        }
        output_str << std::endl;
    }
    return output_str.str();
}
