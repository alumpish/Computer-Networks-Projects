#include "network.hpp"

#include <iostream>

Network::Network(Graph& graph) : graph_(graph) {}

int minDistance(std::vector<int> dist, std::vector<bool> visited) {
    int min = INT_MAX, min_index;

    for (int v = 0; v < dist.size(); ++v) {
        if (!visited[v] && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }

    return min_index;
}

void Network::lsrp() {
    int n = graph_.getNodesCount();
    for (int i = 0; i < n; ++i) {
        lsrp(i + 1);
    }
}

void Network::lsrp(int source) {
    int n = graph_.getNodesCount();
    std::vector<int> dist(n, INT_MAX);
    std::vector<bool> visited(n, false);
    std::vector<int> parent(n, -1);

    dist[source - 1] = 0;
    parent[source - 1] = source;

    for (int i = 0; i < n - 1; ++i) {
        int u = minDistance(dist, visited);
        visited[u] = true;

        for (int v = 0; v < n; ++v) {
            if (!visited[v] && graph_.getCost(u + 1, v + 1) != -1 &&
                dist[u] != INT_MAX && dist[u] + graph_.getCost(u + 1, v + 1) < dist[v]) {
                dist[v] = dist[u] + graph_.getCost(u + 1, v + 1);
                parent[v] = u + 1;
            }
        }
    }

    std::cout << "Source: " << source << std::endl;
    std::cout << "Node\tDistance\tPath" << std::endl;
    for (int i = 0; i < n; ++i) {
        std::cout << i + 1 << "\t" << dist[i] << "\t\t";
        printPath(parent, i);
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void Network::printPath(std::vector<int> parent, int j) {
    if (parent[j] == -1) {
        return;
    }

    printPath(parent, parent[j] - 1);
    std::cout << j + 1 << " ";
}

void Network::dvrp() {
    int n = graph_.getNodesCount();
    for (int i = 0; i < n; ++i) {
        dvrp(i + 1);
    }
}

void Network::dvrp(int source) {
    int n = graph_.getNodesCount();
    std::vector<int> dist(n, INT_MAX);
    std::vector<bool> visited(n, false);
    std::vector<int> parent(n, -1);

    dist[source - 1] = 0;
    parent[source - 1] = source;

    for (int i = 0; i < n - 1; ++i) {
        int u = minDistance(dist, visited);
        visited[u] = true;

        for (int v = 0; v < n; ++v) {
            if (!visited[v] && graph_.getCost(u + 1, v + 1) != -1 &&
                dist[u] != INT_MAX && dist[u] + graph_.getCost(u + 1, v + 1) < dist[v]) {
                dist[v] = dist[u] + graph_.getCost(u + 1, v + 1);
                parent[v] = u + 1;
            }
        }
    }

    std::cout << "Source: " << source << std::endl;
    std::cout << "Node\tDistance\tPath" << std::endl;
    for (int i = 0; i < n; ++i) {
        std::cout << i + 1 << "\t" << dist[i] << "\t\t";
        printPath(parent, i);
        std::cout << std::endl;
    }
    std::cout << std::endl;
}