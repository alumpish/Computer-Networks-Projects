#include "network_handler.hpp"

#include <functional>

#include "cli.hpp"
#include "network.hpp"

NetworkHandler::NetworkHandler(Network network)
    : network_(network) {}

std::string NetworkHandler::lsrp(const Cli::ArgumentGroups& args) {
    int node = std::stoi(args.getArgumentGroups()[0][0]);
    network_.lsrp(node);
    return "";
}

std::string NetworkHandler::dvrp(const Cli::ArgumentGroups& args) {
    int node = std::stoi(args.getArgumentGroups()[0][0]);
    network_.dvrp(node);
    return "";
}

void NetworkHandler::setupCli(Cli& cli) {
    using Command = Cli::Command;
    auto bind = [this](std::string (NetworkHandler::*f)(const Cli::ArgumentGroups&)) {
        return std::bind(f, this, std::placeholders::_1);
    };

    cli.addCommand(Command("lsrp", bind(&NetworkHandler::lsrp)));
    cli.addCommand(Command("dvrp", bind(&NetworkHandler::dvrp)));
}
