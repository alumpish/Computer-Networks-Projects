#include "command_handler.hpp"

#include <iostream>
#include <string>
#include <vector>

#include "exceptions.hpp"

CommandHandler::CommandHandler(std::istream& input_stream)
    : input_stream_(input_stream) {}

void CommandHandler::addCommand(const std::string& cmd_name, Command* cmd) {
    commands_map_[cmd_name] = cmd;
}

void CommandHandler::runSingleCommand() {
    std::string cmd_name;
    input_stream_ >> cmd_name;
    Command* command = commands_map_[cmd_name];

    std::vector<std::string> input_args;
    input_args.reserve(command->getArgsCount());

    while (input_args.size() < command->getArgsCount()) {
        std::string temp;
        input_stream_ >> temp;
        input_args.push_back(temp);
    }

    if (!command->matchArgsList(input_args))
        throw Err503();

    command->execCommand(input_args);
}

CommandHandler::~CommandHandler() {
    for (const auto& cmd_pair : commands_map_)
        delete cmd_pair.second;
}
