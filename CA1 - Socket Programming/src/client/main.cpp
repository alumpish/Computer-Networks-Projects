#include "client.hpp"
#include "timer.hpp"

int main() {
    Timer timer;
    Client client(timer);
    client.run();
}