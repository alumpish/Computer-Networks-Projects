# CA3 - Routing Protocols

## Contents

- [Code Documentation](#code-documentation)
  - [Command Line Interface](#command-line-interface)
  - [Graph](#graph)
  - [Graph's Command Handling](#graphs-command-handling)
  - [Network's Command Handling](#networks-command-handling)

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

## Network's Command Handling

This is exactly similar to `GraphHandler`; So similar that we should have made an interface ot something out of it!!<br>
We will finish this section here as there is no need for further explanations.



## Network

The Network class represents a network and provides various methods for performing routing algorithms. Here is a summary of the class and its methods:

### `Network`

- `Network(Graph& graph)`: The constructor takes a Graph object as a parameter and initializes the graph_ member variable with it. The Graph object represents the network graph.

- `int minDistance(std::vector<int> dist, std::vector<bool> visited)`: This method calculates and returns the node with the minimum distance from a set of nodes. It takes the dist vector (representing distances from a source node) and the visited vector (marking visited nodes) as parameters.

- `void printDist(std::vector<int> dist)`: This method prints the distances of all nodes from a source node. It takes the dist vector as a parameter.

- `void printPath(std::vector<int> parent, int j)`: This method prints the path from a source node to a specific node j. It takes the parent vector (representing parent nodes in the shortest path tree) and the node j as parameters.

- `void printNode(int source, std::vector<int> dist, std::vector<int> parent, int n)`: This method prints the shortest paths from a source node to all other nodes. It takes the source node, dist vector, parent vector, and the total number of nodes n as parameters.

- `void lsrp(int source)`: This method performs the LSRP algorithm with a specified source node. It computes the shortest paths from the given source node to all other nodes and prints the results.

- `void lsrp()`: (LSRP) algorithm without specifying a source node.

- `void dvrp(int source)`: This method performs the DVRP algorithm with a specified source node. It computes the shortest paths from the given source node to all other nodes and prints the results.

- `void dvrp()`: (DVRP) algorithm without specifying a source node.

The Network class relies on a Graph object to represent the network topology. The specific implementation of the Graph class is not provided in the code snippet.

&nbsp;

### `LSRP Algorithm`


```cpp
void Network::lsrp(int source) {
    int n = graph_.getNodesCount();
    std::vector<int> dist(n, INT_MAX);
    std::vector<bool> visited(n, false);
    std::vector<int> parent(n, -1);

    dist[source - 1] = 0;

    for (int i = 0; i < n - 1; ++i) {
        int u = minDistance(dist, visited);
        visited[u] = true;

        for (int v = 0; v < n; ++v) {
            if (!visited[v] && graph_.getCost(u + 1, v + 1) != -1 &&
                dist[u] != INT_MAX && dist[u] + graph_.getCost(u + 1, v + 1) < dist[v]) {
                dist[v] = dist[u] + graph_.getCost(u + 1, v + 1);
                parent[v] = u + 1;
            }
        }

        std::cout << "Iteration " << i << " Source:" << source << std::endl;
        printDist(dist);
        std::cout << "-----------------------\n";
    }

    std::cout << "\n";
    printNode(source, dist, parent, n);
    std::cout << "=======================\n\n";
}
```

The given code implements the Link State Routing Protocol **(LSRP)** algorithm within the Network class. The lsrp method calculates the shortest paths from a specified source node to all other nodes in a network graph. Here's an overview of what the code does:

It retrieves the total number of nodes in the network graph using the getNodesCount method of the graph_ member variable.

Three vectors are initialized:

`dist`: A vector that represents the shortest distance from the source node to each node in the network. All elements are initially set to INT_MAX (infinity).

`visited`: A vector that keeps track of visited nodes during the algorithm's execution. All elements are initially set to false.

`parent`: A vector that stores the parent node for each node in the computed shortest path tree. All elements are initially set to `-1`.
The distance from the source node to itself is set to `0` in the dist vector.

The algorithm enters a loop that iterates `n - 1` times, where `n` is the total number of nodes.

Within each iteration, the algorithm selects the node `u` with the minimum distance from the set of unvisited nodes.

The node `u` is marked as visited by setting the corresponding element in the visited vector to true.

Another loop iterates over all nodes in the network to examine unvisited neighbor nodes of node `u`.

Within the second loop, there is a condition that checks if a neighbor node `v` is unvisited, there is a valid connection between nodes `u` and `v`, the distance to node `u` is not `INT_MAX`, and the distance through node `u` to node `v` is shorter than the current known distance to node `v`. If the condition is true, the `distance` and `parent` values for node `v` are updated.

After each iteration of the outer loop, the current state of the dist vector is printed, showing the distances of all nodes from the source node.

Once the loop completes, the method prints the shortest paths from the source node to all other nodes using the printNode method.

The method outputs a visual separator to indicate the end of the algorithm execution.

In summary, the lsrp method uses the LSRP algorithm to compute the shortest paths from a given source node to all other nodes in the network and provides visibility into the algorithm's progress and results through console output.

&nbsp;

### `DVRP Algorithm`


```cpp
void Network::dvrp(int source) {
    int n = graph_.getNodesCount();
    std::vector<int> dist(n, INT_MAX);
    std::vector<int> parent(n, -1);

    dist[source - 1] = 0;

    for (int i = 0; i < n - 1; ++i) {
        for (int u = 0; u < n; ++u) {
            for (int v = 0; v < n; ++v) {
                if (graph_.getCost(u + 1, v + 1) != -1 &&
                    dist[u] != INT_MAX && dist[u] + graph_.getCost(u + 1, v + 1) < dist[v]) {
                    dist[v] = dist[u] + graph_.getCost(u + 1, v + 1);
                    parent[v] = u + 1;
                }
            }
        }
    }

    printNode(source, dist, parent, n);
}
```

The given code implements the Distance Vector Routing Protocol **(DVRP)** algorithm within the Network class. The dvrp method calculates the shortest paths from a specified source node to all other nodes in a network graph. Here's an overview of what the code does:

It retrieves the total number of nodes in the network graph using the getNodesCount method of the graph_ member variable.

Two vectors are initialized like the LSRP algorithm. The `visited` vector is not needed in this algorithm.

The algorithm enters a loop that iterates `n - 1` times, where `n` is the total number of nodes.

Within each iteration, there are two nested loops that iterate over all nodes in the network. These loops examine each possible pair of nodes `u` and `v`.

Within the innermost loop, there is a condition that checks if there is a valid connection between nodes `u` and `v`, the distance to node `u` is not `INT_MAX`, and the distance through node `u` to node `v` is shorter than the current known distance to node `v`. If the condition is true, the `distance` and `parent` values for node `v` are updated.

After the loop completes, the method prints the shortest paths from the source node to all other nodes using the printNode method.

In summary, the dvrp method uses the DVRP algorithm to compute the shortest paths from a given source node to all other nodes in the network and provides the computed distances and parent information for each node.

&nbsp;

## Results
As mentioned in the project description, the program should be able to handle the following commands:

Input:
```
topology 1-5-6 1-7-7 2-6-2 2-7-13 3-8-4 3-7-1 3-12-8 4-6-19 4-10-11 5-10-3 5-1-6 6-2-2 6-13-4 6-4-19 6-11-25 6-8-17 7-2-13 7-3-1 7-1-7 7-12-8 8-3-4 8-11-16 8-6-17 9-12-5 9-13-7 10-4-11 10-5-3 10-12-12 11-8-16 11-6-25 12-7-8 12-3-8 12-10-12 12-9-5 13-6-4 13-9-7
show
```

output:
```
OK
     |    1    2    3    4    5    6    7    8    9   10   11   12   13
-----------------------------------------------------------------------
    1|    0   -1   -1   -1    6   -1    7   -1   -1   -1   -1   -1   -1
    2|   -1    0   -1   -1   -1    2   13   -1   -1   -1   -1   -1   -1
    3|   -1   -1    0   -1   -1   -1    1    4   -1   -1   -1    8   -1
    4|   -1   -1   -1    0   -1   19   -1   -1   -1   11   -1   -1   -1
    5|    6   -1   -1   -1    0   -1   -1   -1   -1    3   -1   -1   -1
    6|   -1    2   -1   19   -1    0   -1   17   -1   -1   25   -1    4
    7|    7   13    1   -1   -1   -1    0   -1   -1   -1   -1    8   -1
    8|   -1   -1    4   -1   -1   17   -1    0   -1   -1   16   -1   -1
    9|   -1   -1   -1   -1   -1   -1   -1   -1    0   -1   -1    5    7
   10|   -1   -1   -1   11    3   -1   -1   -1   -1    0   -1   12   -1
   11|   -1   -1   -1   -1   -1   25   -1   16   -1   -1    0   -1   -1
   12|   -1   -1    8   -1   -1   -1    8   -1    5   12   -1    0   -1
   13|   -1   -1   -1   -1   -1    4   -1   -1    7   -1   -1   -1    0
```

Then we can run the LSRP algorithm on the network with the following command:
```
lsrp
```

Result for node 1:
```
Iteration 0 Source:1
Node	1	2	3	4	5	6	7	8	9	10	11	12	13	
Dest	0	-	-	-	6	-	7	-	-	-	-	-	-	
-----------------------
Iteration 1 Source:1
Node	1	2	3	4	5	6	7	8	9	10	11	12	13	
Dest	0	-	-	-	6	-	7	-	-	9	-	-	-	
-----------------------
Iteration 2 Source:1
Node	1	2	3	4	5	6	7	8	9	10	11	12	13	
Dest	0	20	8	-	6	-	7	-	-	9	-	15	-	
-----------------------
Iteration 3 Source:1
Node	1	2	3	4	5	6	7	8	9	10	11	12	13	
Dest	0	20	8	-	6	-	7	12	-	9	-	15	-	
-----------------------
Iteration 4 Source:1
Node	1	2	3	4	5	6	7	8	9	10	11	12	13	
Dest	0	20	8	20	6	-	7	12	-	9	-	15	-	
-----------------------
Iteration 5 Source:1
Node	1	2	3	4	5	6	7	8	9	10	11	12	13	
Dest	0	20	8	20	6	29	7	12	-	9	28	15	-	
-----------------------
Iteration 6 Source:1
Node	1	2	3	4	5	6	7	8	9	10	11	12	13	
Dest	0	20	8	20	6	29	7	12	20	9	28	15	-	
-----------------------
Iteration 7 Source:1
Node	1	2	3	4	5	6	7	8	9	10	11	12	13	
Dest	0	20	8	20	6	29	7	12	20	9	28	15	27	
-----------------------
Iteration 8 Source:1
Node	1	2	3	4	5	6	7	8	9	10	11	12	13	
Dest	0	20	8	20	6	29	7	12	20	9	28	15	27	
-----------------------
Iteration 9 Source:1
Node	1	2	3	4	5	6	7	8	9	10	11	12	13	
Dest	0	20	8	20	6	22	7	12	20	9	28	15	27	
-----------------------
Iteration 10 Source:1
Node	1	2	3	4	5	6	7	8	9	10	11	12	13	
Dest	0	20	8	20	6	22	7	12	20	9	28	15	26	
-----------------------
Iteration 11 Source:1
Node	1	2	3	4	5	6	7	8	9	10	11	12	13	
Dest	0	20	8	20	6	22	7	12	20	9	28	15	26	
-----------------------

Source: 1
Node	Distance	Path
1	0		1
2	20		1 > 7 > 2
3	8		1 > 7 > 3
4	20		1 > 5 > 10 > 4
5	6		1 > 5
6	22		1 > 7 > 2 > 6
7	7		1 > 7
8	12		1 > 7 > 3 > 8
9	20		1 > 7 > 12 > 9
10	9		1 > 5 > 10
11	28		1 > 7 > 3 > 8 > 11
12	15		1 > 7 > 12
13	26		1 > 7 > 2 > 6 > 13

=======================
```

All nodes results are in out.txt file in test directory.

&nbsp;

Execution time for LSRP algorithm for all nodes:
```
LSRP takes 222834076 ns
```

&nbsp;

We run the DVRP algorithm on the network with the following command:
```
dvrp
```

Results are same as LSRP algorithm.

&nbsp;

Execution time for DVRP algorithm for all nodes:
```
DVRP takes 72174792 ns
```

Then we remove the link between nodes 4 and 10 with the following command and show the network topology:
```
remove 4-10
show
```

output:
```
OK
     |    1    2    3    4    5    6    7    8    9   10   11   12   13
-----------------------------------------------------------------------
    1|    0   -1   -1   -1    6   -1    7   -1   -1   -1   -1   -1   -1
    2|   -1    0   -1   -1   -1    2   13   -1   -1   -1   -1   -1   -1
    3|   -1   -1    0   -1   -1   -1    1    4   -1   -1   -1    8   -1
    4|   -1   -1   -1    0   -1   19   -1   -1   -1   -1   -1   -1   -1
    5|    6   -1   -1   -1    0   -1   -1   -1   -1    3   -1   -1   -1
    6|   -1    2   -1   19   -1    0   -1   17   -1   -1   25   -1    4
    7|    7   13    1   -1   -1   -1    0   -1   -1   -1   -1    8   -1
    8|   -1   -1    4   -1   -1   17   -1    0   -1   -1   16   -1   -1
    9|   -1   -1   -1   -1   -1   -1   -1   -1    0   -1   -1    5    7
   10|   -1   -1   -1   -1    3   -1   -1   -1   -1    0   -1   12   -1
   11|   -1   -1   -1   -1   -1   25   -1   16   -1   -1    0   -1   -1
   12|   -1   -1    8   -1   -1   -1    8   -1    5   12   -1    0   -1
   13|   -1   -1   -1   -1   -1    4   -1   -1    7   -1   -1   -1    0
```

We run the LSRP algorithm again and can see changes in the results of node 1 for destination 4:
```
Iteration 0 Source:1
Node	1	2	3	4	5	6	7	8	9	10	11	12	13	
Dest	0	-	-	-	6	-	7	-	-	-	-	-	-	
-----------------------
Iteration 1 Source:1
Node	1	2	3	4	5	6	7	8	9	10	11	12	13	
Dest	0	-	-	-	6	-	7	-	-	9	-	-	-	
-----------------------
Iteration 2 Source:1
Node	1	2	3	4	5	6	7	8	9	10	11	12	13	
Dest	0	20	8	-	6	-	7	-	-	9	-	15	-	
-----------------------
Iteration 3 Source:1
Node	1	2	3	4	5	6	7	8	9	10	11	12	13	
Dest	0	20	8	-	6	-	7	12	-	9	-	15	-	
-----------------------
Iteration 4 Source:1
Node	1	2	3	4	5	6	7	8	9	10	11	12	13	
Dest	0	20	8	-	6	-	7	12	-	9	-	15	-	
-----------------------
Iteration 5 Source:1
Node	1	2	3	4	5	6	7	8	9	10	11	12	13	
Dest	0	20	8	-	6	29	7	12	-	9	28	15	-	
-----------------------
Iteration 6 Source:1
Node	1	2	3	4	5	6	7	8	9	10	11	12	13	
Dest	0	20	8	-	6	29	7	12	20	9	28	15	-	
-----------------------
Iteration 7 Source:1
Node	1	2	3	4	5	6	7	8	9	10	11	12	13	
Dest	0	20	8	-	6	29	7	12	20	9	28	15	27	
-----------------------
Iteration 8 Source:1
Node	1	2	3	4	5	6	7	8	9	10	11	12	13	
Dest	0	20	8	-	6	22	7	12	20	9	28	15	27	
-----------------------
Iteration 9 Source:1
Node	1	2	3	4	5	6	7	8	9	10	11	12	13	
Dest	0	20	8	41	6	22	7	12	20	9	28	15	26	
-----------------------
Iteration 10 Source:1
Node	1	2	3	4	5	6	7	8	9	10	11	12	13	
Dest	0	20	8	41	6	22	7	12	20	9	28	15	26	
-----------------------
Iteration 11 Source:1
Node	1	2	3	4	5	6	7	8	9	10	11	12	13	
Dest	0	20	8	41	6	22	7	12	20	9	28	15	26	
-----------------------

Source: 1
Node	Distance	Path
1	0		1
2	20		1 > 7 > 2
3	8		1 > 7 > 3
4	41		1 > 7 > 2 > 6 > 4
5	6		1 > 5
6	22		1 > 7 > 2 > 6
7	7		1 > 7
8	12		1 > 7 > 3 > 8
9	20		1 > 7 > 12 > 9
10	9		1 > 5 > 10
11	28		1 > 7 > 3 > 8 > 11
12	15		1 > 7 > 12
13	26		1 > 7 > 2 > 6 > 13

=======================
```

Execution time for LSRP algorithm for all nodes:
```
LSRP takes 252839572 ns
```

DVRP algorithm results are same as LSRP algorithm.

Execution time for DVRP algorithm for all nodes:
```
DVRP takes 67539192 ns
```

### `Conclusion`

summary of execution times for LSRP and DVRP algorithms for all nodes:

First run:
LSRP takes 222834076 ns

DVRP takes 72174792 ns

&nbsp;


Second run:
LSRP takes 252839572 ns

DVRP takes 67539192 ns

&nbsp;

Algorithms complexity are same but LSRP is slower than DVRP because of the extra work it does to find the shortest path tree because of matrix implemetation of Graph.


