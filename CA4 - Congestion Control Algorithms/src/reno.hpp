#ifndef RENO_HPP
#define RENO_HPP

#include <fstream>
#include <string>

class Reno {
    enum Mode {
        SLOW_START,
        CONGESTION_AVOIDANCE,
        FAST_RECOVERY,
        FAST_TRANSIMIT
    };

public:
    Reno(int cwnd, int ssthresh, int fileSize);

    void sendData();
    void onPacketLoss(int count);
    void onRTTUpdate(int newRTT);
    void log(std::ofstream& dataFile) const;
    void retransmit();

    void run();

private:
    Mode mode_;         // Current mode
    int cwnd_;          // Congestion window
    int ssthresh_;      // Slow start threshold
    int rtt_;           // Round trip time
    int losstresh_;     // Loss threshold
    double lossScale_;  // Loss scale
    int fileSize_;      // File size
    int unaknowledged_; // Unaknowledged packets
    int time_;          // Time
    int retransRemain_;

    double lossProbability();
};

#endif