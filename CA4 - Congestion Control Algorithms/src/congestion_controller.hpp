#ifndef CONGESTION_CONTROLLER_HPP
#define CONGESTION_CONTROLLER_HPP

#include <fstream>
#include <string>

class CongestionController {
public:
    CongestionController(int fileSize, std::string connectionName, double lossScale);
    virtual void sendData() = 0;
    virtual void onPacketLoss(int count) = 0;
    virtual void log(std::ofstream& dataFile) const = 0;
    virtual void run() = 0;

protected:
    std::string connectionName_; // Name of Connection
    int cwnd_;                   // Congestion window
    int ssthresh_;               // Slow start threshold
    double rtt_;                 // Round trip time
    int losstresh_;              // Loss threshold
    double lossScale_;           // Loss scale
    int fileSize_;               // File size
    double time_;                // Time
    int retransRemain_;          // Remained size of unacknowledged packet

    virtual double lossProbability() = 0;
};

#endif