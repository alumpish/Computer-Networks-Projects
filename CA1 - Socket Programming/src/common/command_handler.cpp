#include "command_handler.hpp"

#include <iostream>
#include <string>
#include <vector>

#include "exceptions.hpp"

CommandHandler::CommandHandler(std::istream& input_stream)
    : input_stream_(input_stream),
      init_root_(new CommandHandler::CommandNode),
      root_(init_root_) {}

CommandHandler::CommandHandler(CommandHandler& other)
    : input_stream_(other.input_stream_),
      root_(other.root_),
      init_root_(other.init_root_) {}

void CommandHandler::addCommand(const std::string& cmd_name, Command* cmd) {
    CommandHandler::CommandNode* new_node = new CommandHandler::CommandNode;
    new_node->current_command = cmd;
    root_->sub_nodes[cmd_name] = new_node;
}

void CommandHandler::runSingleCommand() {
    std::string cmd_name;
    input_stream_ >> cmd_name;
    CommandHandler::CommandNode* new_root = root_->sub_nodes[cmd_name];
    Command* command = new_root->current_command;

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
    root_ = new_root;
}

void CommandHandler::resetRoot() {
    root_ = init_root_;
}

CommandHandler CommandHandler::operator[](const std::string& cmd_name) {
    CommandHandler result(*this);
    result.root_ = result.root_->sub_nodes[cmd_name];
    return result;
}

CommandHandler::~CommandHandler() {
}
