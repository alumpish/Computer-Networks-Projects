#include "command_handler.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "exceptions.hpp"

CommandHandler::CommandHandler(std::istream& input_stream)
    : input_stream_(input_stream) {
    init_root_ = new CommandHandler::CommandNode;
    root_ = init_root_;
}

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
    auto new_root_itr = root_->sub_nodes.find(cmd_name);
    if (new_root_itr == root_->sub_nodes.end())
        throw CommandNotFound();
    CommandHandler::CommandNode* new_root = new_root_itr->second;
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

    root_ = new_root;
    command->execCommand(input_args);
}

void CommandHandler::resetRoot() {
    root_ = init_root_;
}

std::string CommandHandler::currentLevelCommandsToString() const {
    std::ostringstream result;
    for (const auto& cmd_map_pair : root_->sub_nodes) {
        result << cmd_map_pair.first << " : " << cmd_map_pair.second->current_command->help();
        result << std::endl;
    }
    return result.str();
}

std::string CommandHandler::allLevelsCommandsToString() const {
    // TODO
}

CommandHandler CommandHandler::operator[](const std::string& cmd_name) {
    CommandHandler result(*this);
    result.root_ = result.root_->sub_nodes[cmd_name];
    return result;
}

void CommandHandler::deleteCommands() {
    for (auto& sub_nodes_pair : init_root_->sub_nodes)
        delete sub_nodes_pair.second;
}

void CommandHandler::deleteAll() {
    delete init_root_;
}

CommandHandler::CommandNode::~CommandNode() {
    delete current_command;
    for (auto& sub_nodes_pair : sub_nodes)
        delete sub_nodes_pair.second;
}
