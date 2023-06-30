#include "bbr.hpp"
#include "new_reno.hpp"
#include "reno.hpp"

const int FILE_SIZE = 1000;

int main() {
    Reno reno1(1, 511, 0.015, FILE_SIZE, "Reno_1");
    reno1.run();

    Reno reno2(1, 1023, 0.015, FILE_SIZE, "Reno_2");
    reno2.run();

    NewReno newReno1(1, 511, 0.015, FILE_SIZE, "NewReno_1");
    newReno1.run();

    NewReno newReno2(1, 1023, 0.015, FILE_SIZE, "NewReno_2");
    newReno2.run();

    BBR bbr1(10, 10, 0.015, FILE_SIZE, "BBR_1");
    bbr1.run();

    BBR bbr2(10, 10, 0.02, FILE_SIZE, "BBR_2");
    bbr2.run();

    return 0;
}