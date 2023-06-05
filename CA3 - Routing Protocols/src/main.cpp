#include "cli.hpp"
#include "graph.hpp"
#include "graph_handler.hpp"
#include "network.hpp"
#include "network_handler.hpp"

int main() {
    Graph graph;
    Cli cli;
    GraphHandler graph_handler(graph);
    Network network(graph);
    NetworkHandler network_handler(network);
    graph_handler.setupCli(cli);
    network_handler.setupCli(cli);
    cli.run();
    return 0;
}
