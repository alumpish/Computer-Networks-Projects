#include "graph_handler.hpp"

#include <functional>
#include <string>
#include <vector>

#include "cli.hpp"
#include "graph.hpp"

GraphHandler::GraphHandler(Graph& graph)
    : graph_(graph) {}

std::string GraphHandler::topology(const Cli::ArgumentGroups& args) {
    auto addLink = [](const Cli::ArgumentGroups::ArgumentGroup arg_group, Graph& graph) -> void {
        int src = std::stoi(arg_group[0]);
        int dest = std::stoi(arg_group[1]);
        int cost = std::stoi(arg_group[2]);
        if (src == dest)
            ; // Throw exception
        graph.addLink(src, dest, cost);
    };

    auto findNodesCount = [](const Cli::ArgumentGroups args) -> int {
        int max_node_id = 0;
        for (const auto arg_group : args.getArgumentGroups()) {
            int src = std::stoi(arg_group[0]);
            int dest = std::stoi(arg_group[1]);
            max_node_id = std::max(max_node_id, std::max(src, dest));
        }
        return max_node_id;
    };

    graph_.setNodesCount(findNodesCount(args));
    for (const auto arg_group : args.getArgumentGroups())
        addLink(arg_group, graph_);

    return "OK";
}

std::string GraphHandler::show(const Cli::ArgumentGroups& args) {
    return graph_.toString();
}

std::string GraphHandler::modify(const Cli::ArgumentGroups& args) {
    const auto arg_group = args.getArgumentGroups()[0];
    graph_.changeLinkCost(std::stoi(arg_group[0]), std::stoi(arg_group[1]), std::stoi(arg_group[2]));
    return "OK";
}

std::string GraphHandler::remove(const Cli::ArgumentGroups& args) {
    const auto arg_group = args.getArgumentGroups()[0];
    graph_.removeLink(std::stoi(arg_group[0]), std::stoi(arg_group[1]));
    return "OK";
}

void GraphHandler::setupCli(Cli& cli) {
    using Command = Cli::Command;
    auto bind = [this](std::string (GraphHandler::*f)(const Cli::ArgumentGroups&)) {
        return std::bind(f, this, std::placeholders::_1);
    };

    cli.addCommand(Command("topology", bind(&GraphHandler::topology)));
    cli.addCommand(Command("show", bind(&GraphHandler::show)));
    cli.addCommand(Command("modify", bind(&GraphHandler::modify)));
    cli.addCommand(Command("remove", bind(&GraphHandler::remove)));
}
