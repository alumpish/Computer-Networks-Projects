#include "network.hpp"

#include <limits.h>

#include <chrono>
#include <iostream>
#include <vector>

Network::Network(Graph& graph) : graph_(graph) {}

int Network::minDistance(std::vector<int> dist, std::vector<bool> visited) {
    int min = INT_MAX, min_index = 0;

    for (int v = 0; v < dist.size(); ++v) {
        if (!visited[v] && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }

    return min_index;
}

void Network::printDist(std::vector<int> dist) {
    std::cout << "Node\t";
    for (int i = 0; i < dist.size(); ++i) {
        std::cout << i + 1 << "\t";
    }
    std::cout << std::endl
              << "Dest\t";
    for (int i = 0; i < dist.size(); ++i) {
        if (dist[i] == INT_MAX)
            std::cout << "-"
                      << "\t";
        else
            std::cout << dist[i] << "\t";
    }
    std::cout << std::endl;
}

void Network::printPath(std::vector<int> parent, int j) {
    if (parent[j] == -1) {
        return;
    }

    printPath(parent, parent[j] - 1);
    std::cout << " > " << j + 1;
}

void Network::printNode(int source, std::vector<int> dist, std::vector<int> parent, int n) {
    std::cout << "Source: " << source << std::endl;
    std::cout << "Node\tDistance\tPath" << std::endl;
    for (int i = 0; i < n; ++i) {
        if (dist[i] == INT_MAX) {
            std::cout << i + 1 << "\t"
                      << "-"
                      << "\t\t-";
        }
        else {
            std::cout << i + 1 << "\t" << dist[i] << "\t\t" << source;
            printPath(parent, i);
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void Network::lsrp() {
    auto begin = std::chrono::high_resolution_clock::now();

    int n = graph_.getNodesCount();
    for (int i = 0; i < n; ++i) {
        lsrp(i + 1);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "LSRP takes " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() << " ns" << std::endl;
}

void Network::lsrp(int source) {
    int n = graph_.getNodesCount();
    std::vector<int> dist(n, INT_MAX);
    std::vector<bool> visited(n, false);
    std::vector<int> parent(n, -1);

    dist[source - 1] = 0;

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

        std::cout << "Iteration " << i << " Source:" << source << std::endl;
        printDist(dist);
        std::cout << "-----------------------\n";
    }

    std::cout << "\n";
    printNode(source, dist, parent, n);
    std::cout << "=======================\n\n";
}

void Network::dvrp() {
    auto begin = std::chrono::high_resolution_clock::now();

    int n = graph_.getNodesCount();
    for (int i = 0; i < n; ++i) {
        dvrp(i + 1);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "DVRP takes " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() << " ns" << std::endl;
}

void Network::dvrp(int source) {
    int n = graph_.getNodesCount();
    std::vector<int> dist(n, INT_MAX);
    std::vector<int> parent(n, -1);

    dist[source - 1] = 0;

    for (int i = 0; i < n - 1; ++i) {
        for (int u = 0; u < n; ++u) {
            for (int v = 0; v < n; ++v) {
                if (graph_.getCost(u + 1, v + 1) != -1 &&
                    dist[u] != INT_MAX && dist[u] + graph_.getCost(u + 1, v + 1) < dist[v]) {
                    dist[v] = dist[u] + graph_.getCost(u + 1, v + 1);
                    parent[v] = u + 1;
                }
            }
        }
    }

    printNode(source, dist, parent, n);
}
