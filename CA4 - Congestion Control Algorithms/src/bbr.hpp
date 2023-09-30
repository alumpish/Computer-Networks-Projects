#ifndef BBR_HPP
#define BBR_HPP
#include <fstream>
#include <iostream>

#include "congestion_controller.hpp"

class BBR : public CongestionController {
    enum Mode {
        STARTUP,
        DRAIN,
        PROBE_BANDWIDTH,
        PROBE_RTT
    };

public:
    BBR(double max_bw, double min_rtt, double lossScale, int fileSize, std::string connectionName);
    void updateBandwidth(double bandwidth);
    double getSendingRate();
    void updateCongestionWindow(bool is_loss);
    void onBottleneck();
    void onPacketLoss(int count);
    void sendData();
    void log(std::ofstream& dataFile) const;
    void run();

private:
    Mode mode_;
    int target_cwnd_;
    double max_bandwidth_;
    double min_rtt_;
    double pacing_gain_;
    double gain_cycle_;
    int unaknowledged_;

    double lossProbability();
    double calculateCwndGain();
    void updateRTT();
};

#endif // BBR_HPP