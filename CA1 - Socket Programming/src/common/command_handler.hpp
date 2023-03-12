#ifndef COMMAND_HANDLER_HPP
#define COMMAND_HANDLER_HPP

#include <functional>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

using exec_func = std::function<void(const std::vector<std::string>&)>;

class CommandHandler {
public:
    struct Command {
        std::vector<std::string> args_regex;
        std::string help;
        exec_func func;

        int getArgsCount() const { return args_regex.size(); };
        void execCommand(const std::vector<std::string>& input_args) { func(input_args); }
        std::string help() const { return help; };
        bool matchArgsList(const std::vector<std::string>& input_args) const {
            if (input_args.size() != args_regex.size())
                return false;
            for (int i = 0; i < args_regex.size(); ++i)
                if (!std::regex_match(input_args[i], std::regex(args_regex[i])))
                    return false;
            return true;
        }
    };

    CommandHandler(std::istream& input_stream);
    void addCommand(const std::string& cmd_name, Command* cmd);
    void runSingleCommand();
    ~CommandHandler();

private:
    std::istream& input_stream_;
    std::unordered_map<std::string, CommandHandler::Command*> commands_map_;
};

#endif