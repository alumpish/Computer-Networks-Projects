#include <cli.hpp>
#include <sstream>
#include <string>

Cli::ArgumentGroups::ArgumentGroups() {}

void Cli::ArgumentGroups::parse(const std::string& args_line) {
    std::istringstream args_line_str(args_line);
    std::string temp_arg_group;
    while (std::getline(args_line_str, temp_arg_group, ' ')) {
        ArgumentGroup current_group;
        std::istringstream group_str(temp_arg_group);
        std::string arg_inside_group_temp;
        while (std::getline(group_str, arg_inside_group_temp, '-'))
            current_group.push_back(arg_inside_group_temp);

        arg_groups_.push_back(current_group);
    }
}

std::vector<Cli::ArgumentGroups::ArgumentGroup> Cli::ArgumentGroups::getArgumentGroups() const {
    return arg_groups_;
}

Cli::Command::Command(const std::string& cmd_name, ExecFunc exec_func)
    : name_(cmd_name),
      exec_func_(exec_func) {}

std::string Cli::Command::run(const std::string& args_line) {
    ArgumentGroups args_groups;
    args_groups.parse(args_line);
    return exec_func_(args_groups);
}

bool Cli::Command::matchName(const std::string& name) const {
    return name_ == name;
}

Cli::Cli() {}

void Cli::run() {
    std::string current_command;
    while (input_stream_ >> current_command) {
        try {
            auto found_command = findMatchingCommand(current_command);
            std::string current_args;
            input_stream_ >> current_args;
            output_stream_ << found_command.run(current_args);
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << '\n';
        }
    }
}

void Cli::addCommand(Command new_command) {
    commands_list_.push_back(new_command);
}

const Cli::Command& Cli::findMatchingCommand(const std::string& command_name) const {
    for (auto& current_command : commands_list_)
        if (current_command.matchName(command_name))
            return current_command;
    // Throw NotFound
}
