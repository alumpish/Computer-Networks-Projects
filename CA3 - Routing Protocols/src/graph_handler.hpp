#ifndef GRAPH_HANDLER_HPP
#define GRAPH_HANDLER_HPP

#include <string>

#include "cli.hpp"
#include "graph.hpp"

class GraphHandler {
public:
    GraphHandler(Graph& graph);

    std::string topology(const Cli::ArgumentGroups& args);
    std::string show(const Cli::ArgumentGroups& args);
    std::string modify(const Cli::ArgumentGroups& args);
    std::string remove(const Cli::ArgumentGroups& args);

    void setupCli(Cli& cli);

private:
    Graph& graph_;
};

#endif