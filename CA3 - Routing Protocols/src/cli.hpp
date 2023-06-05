#ifndef CLI_HPP
#define CLI_HPP

#include <functional>
#include <iostream>
#include <string>
#include <vector>

class Cli {
public:
    class ArgumentGroups {
    public:
        using Argument = std::string;
        using ArgumentGroup = std::vector<Argument>;

        ArgumentGroups();

        void parse(const std::string& args_line);
        std::vector<ArgumentGroup> getArgumentGroups() const;

    private:
        std::vector<ArgumentGroup> arg_groups_;
    };

    class Command {
    public:
        using ExecFunc = std::function<std::string(const ArgumentGroups&)>;

        Command(const std::string& cmd_name, ExecFunc exec_func);

        std::string run(const std::string& args_line);
        bool matchName(const std::string& name) const;

    private:
        std::string name_;
        ExecFunc exec_func_;
    };

    Cli();

    void run();
    void addCommand(Command new_command);

private:
    std::vector<Command> commands_list_;
    std::istream& input_stream_ = std::cin;
    std::ostream& output_stream_ = std::cout;

    const Command& findMatchingCommand(const std::string& command_name) const;
};

#endif