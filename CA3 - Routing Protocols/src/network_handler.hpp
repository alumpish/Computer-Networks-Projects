#ifndef NETWORK_HANDLER_HPP
#define NETWORK_HANDLER_HPP

#include "cli.hpp"
#include "network.hpp"

class NetworkHandler {
public:
    NetworkHandler(Network network);

    std::string lsrp(const Cli::ArgumentGroups& args);
    std::string dvrp(const Cli::ArgumentGroups& args);

    void setupCli(Cli& cli);

private:
    Network network_;
};

#endif