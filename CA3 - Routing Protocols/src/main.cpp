#include "cli.hpp"
#include "graph.hpp"
#include "graph_handler.hpp"

int main() {
    Graph graph;
    Cli cli;
    GraphHandler graph_handler(graph);
    graph_handler.setupCli(cli);
    cli.run();
    return 0;
}
