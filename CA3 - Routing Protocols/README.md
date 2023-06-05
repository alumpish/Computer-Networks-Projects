# Computer Networks Project 3

## Contents

- [Code Documentation](#code-documentation)
  - [Command Line Interface](#command-line-interface)
  - [Graph](#graph)
  - [Graph's Command Handlin](#graphs-command-handling)

## Code Documentation

## Command Line Interface

The command line interface is handled through three main classes. `Cli`, `Command` and `ArgumentGroups`. The first one is responsible for handling logistics of the command line and the latter two are data structures with minor responsibilities.

### `Cli`

This class have one default constructor, two public methods and one private utility method. It holds a list of `Command`s and search through it every time a command is entered. The following piece of code does the running job for the command line as it is obvious from its name:

```cpp
void Cli::run() {
    std::string current_command;
    while (input_stream_ >> current_command) {
        try {
            auto found_command = findMatchingCommand(trim(current_command));
            std::string current_args;
            std::getline(input_stream_, current_args);
            output_stream_ << found_command.run(trim(current_args)) << std::endl;
        }
        catch (const CommandNotFound& e) {
            std::string flushed_line;
            std::getline(std::cin, flushed_line);
            std::cout << e.what() << std::endl;
        }
        catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }
}
```

It first tries to find a match for the given command. The `findMatchingCommand` function is a simple loop over the commands list. The it reads the whole line of arguments. For simplicity, every command is thought to be a command name followed by a list of argument groups with size of zero or more. Then it runs the found command and prints the results.

### `Command`

This class is a simple data structure that has name field and a function pointer to run in case of that the command was called. It has a `run` method that simply runs the function pointer with the given arguments and a `matchName` method that is used to check if the input command is this one or not.

### `ArgumentGroups`

This class has two purposes. First is parsing the given line of arguments into its own data structure and second is saving that data structure. The structure of its data is a vector of `ArgumentGroups` which is alias for a vector of `Argument`s. Consider an input arguments of the form `s1-d1-c1 s2-d2-c2`. This is a series of dash separated numbers. every triplet of numbers are considered as an `ArgumentGroup` (This can also be a pair of numbers in case of some commands like `remove`). The following code shows how arguments are parsed into this structure:

```cpp
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
```

The input is first separated by whitespace between each argument group. Then the list of strings from the previous part is separated by dashes. Because of the simplicity of the commands syntax, the complexity of regex and other parsing tools is avoided.

## Graph

The topology of the network is considered as simple graph. This is handled inside the `Graph` class. This class holds an adjacency matrix as the data structure of the graph.

### `Graph`

This class has seven methods that are described below:

- `void setNodesCount(int nodes_count)`: This method sets the number of nodes in the graph. Note that the `Graph` has no field of nodes count or whatsoever. The nodes count is indirectly implied by the size of the adjacency matrix. Every time someone uses this method, the whole structure of the previous graph is re-written, erasing every link information inside it. It is best to use this only immediately after constructing the Graph class.

- `void addLink(int src, int dest, int cost)`: This method adds a link between the given nodes in both direction, from source to node and vice versa. This is because the graph has no direction and the link should be saved for both ways.

- `void changeLinkCost(int src, int dest, int new_cost)`: This one modifies a previously added link. It also adds a new link in case there was no link between the given nodes.

- `void removeLink(int src, int dest)`: No need to explain. It just removes the link!

- `int getCost(int src, int dest) const`: Returns the cost between the given nodes.

- `std::string toString() const`: Gives the adjacency matrix in a beautiful manner. This method is used for the `show` command.

## Graph's Command Handling

We presented a data structure for the graph to be saved in. We also provided a command line interface. But for the commands to actually create and modify graph, we'll need a `GraphHandler` class to setup `Cli` for this purpose.

### `GraphHandler`

This class implements four commands related to the graph: `topology` which will initialize the graph, `show`, `modify`, and `remove`. These methods are with the uniform signature that is required in the `Cli`; That is with `std::string` as their return types and `const Cli::ArgumentGroups&` as their inputs. There is also a `setupCli` method that will map the latter four methods to the command names in `Cli`. Here is the implementation of the `setupCli` method:

```cpp
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
```

The only part that might need an explanation is the bind lambda. This method simply binds the function pointer with an instance of the `GraphHandler` so that it will be useable by the `Cli` class. Then the `addCommand` of the `Cli` class is called to, ofcourse, add the commands.
