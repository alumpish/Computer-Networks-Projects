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
        Command(std::vector<std::string> args_regex, std::string help, exec_func func)
            : args_regex_(args_regex),
              help_(help),
              func_(func) {}
        private: std::vector<std::string> args_regex_;
        private: std::string help_;
        private: exec_func func_;

        public: int getArgsCount() const { return args_regex_.size(); };
        public: void execCommand(const std::vector<std::string>& input_args) { func_(input_args); }
        public: std::string help() const { return help_; };
        public: bool matchArgsList(const std::vector<std::string>& input_args) const {
            if (input_args.size() != args_regex_.size())
                return false;
            for (int i = 0; i < args_regex_.size(); ++i)
                if (!std::regex_match(input_args[i], std::regex(args_regex_[i])))
                    return false;
            return true;
        }
    };

    CommandHandler(std::istream& input_stream);
    CommandHandler(CommandHandler& other);

    void addCommand(const std::string& cmd_name, Command* cmd);
    void runSingleCommand();
    void resetRoot();

    std::string currentLevelCommandsToString() const;
    std::string allLevelsCommandsToString() const;

    CommandHandler operator[](const std::string& cmd_name);

    void deleteCommands();

    ~CommandHandler();

private:
    std::istream& input_stream_;

    struct CommandNode {
        std::unordered_map<std::string, CommandHandler::CommandNode*> sub_nodes;
        CommandHandler::Command* current_command;

        ~CommandNode();
    } *root_, *init_root_;
};

#endif