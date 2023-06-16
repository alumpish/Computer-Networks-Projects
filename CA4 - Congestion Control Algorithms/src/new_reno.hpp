#ifndef NEWRENO_HPP
#define NEWRENO_HPP

#include <fstream>
#include <string>

class NewReno {
    enum Mode {
        SLOW_START,
        CONGESTION_AVOIDANCE,
        FAST_RECOVERY,
        FAST_TRANSIMIT
    };

public:
    NewReno(int cwnd, int ssthresh, int fileSize);

    void sendData();
    void onPacketLoss(int count);
    void onRTTUpdate(int newRTT);
    void onSelectiveAck(int count);
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
    int time_;          // Time
    int retransRemain_;

    double lossProbability();
};

#endif