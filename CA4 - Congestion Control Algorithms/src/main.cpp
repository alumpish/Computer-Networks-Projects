#include "bbr.hpp"
#include "new_reno.hpp"
#include "reno.hpp"

const int FILE_SIZE = 1000;

int main() {
    // Reno connection1(1, 511, FILE_SIZE);
    // connection1.run();

    // NewReno connection2(1, 511, FILE_SIZE);
    // connection2.run();

    BBR connection3(10, 10, FILE_SIZE);
    connection3.run();

    return 0;
}