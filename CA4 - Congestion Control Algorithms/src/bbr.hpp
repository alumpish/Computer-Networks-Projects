#ifndef BBR_HPP
#define BBR_HPP

#include <fstream>
#include <iostream>

class BBR {
    enum Mode {
        STARTUP,
        DRAIN,
        PROBE_BANDWIDTH,
        PROBE_RTT
    };

public:
    BBR(double max_bw, double min_rtt, int fileSize);
    double lossProbability();
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
    double max_bandwidth_;
    double min_rtt_;
    double rtt_;
    int cwnd_;
    int target_cwnd_;
    double pacing_gain_;
    double gain_cycle_;
    int fileSize_;
    int losstresh_;
    double lossScale_;
    double time_;
    int unaknowledged_;

    double calculateCwndGain();
    void updateRTT();
};

#endif // BBR_HPP