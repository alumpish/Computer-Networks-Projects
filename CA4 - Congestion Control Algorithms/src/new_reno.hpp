#ifndef NEWRENO_HPP
#define NEWRENO_HPP
#include <fstream>
#include <string>

#include "congestion_controller.hpp"

class NewReno : public CongestionController {
    enum Mode {
        SLOW_START,
        CONGESTION_AVOIDANCE,
        FAST_RECOVERY,
        FAST_TRANSIMIT
    };

public:
    NewReno(int cwnd, int ssthresh, double lossScale, int fileSize, std::string connectionName);

    void sendData();
    void onPacketLoss(int count);
    void onRTTUpdate(int newRTT);
    void log(std::ofstream& dataFile) const;
    void retransmit();

    void run();

private:
    Mode mode_;         // Current mode
    int unaknowledged_; // Unaknowledged packets

    double lossProbability();
};

#endif