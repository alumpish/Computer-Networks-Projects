#include <sys/select.h>
#include <unistd.h>
#include <thread>

void foo() {
    fd_set working_set;
    FD_SET(STDIN_FILENO, &working_set);
    select(1, &working_set, nullptr, nullptr, nullptr);
}

int main() {
    std::thread t(foo);
    t.join();
}