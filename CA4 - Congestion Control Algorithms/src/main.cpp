#include "bbr.hpp"
#include "new_reno.hpp"
#include "reno.hpp"

const int FILE_SIZE = 1000;

int main() {
    // cwnd = 1, ssthresh = 511, loss_scale = 0.015
    Reno reno1(1, 511, 0.015, FILE_SIZE, "Reno_1");
    reno1.run();

    // cwnd = 1, ssthresh = 1023, loss_scale = 0.015
    Reno reno2(1, 1023, 0.015, FILE_SIZE, "Reno_2");
    reno2.run();

    // cwnd = 1, ssthresh = 511, loss_scale = 0.015
    NewReno newReno1(1, 511, 0.015, FILE_SIZE, "NewReno_1");
    newReno1.run();

    // cwnd = 1, ssthresh = 1023, loss_scale = 0.015
    NewReno newReno2(1, 1023, 0.015, FILE_SIZE, "NewReno_2");
    newReno2.run();

    // max_bw = 10, min_rtt = 10, loss_scale = 0.015
    BBR bbr1(10, 10, 0.015, FILE_SIZE, "BBR_1");
    bbr1.run();

    // max_bw = 10, min_rtt = 10, loss_scale = 0.02
    BBR bbr2(10, 10, 0.02, FILE_SIZE, "BBR_2");
    bbr2.run();

    return 0;
}