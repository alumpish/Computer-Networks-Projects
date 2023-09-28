#include "bbr.hpp"

#include <cmath>
#include <sstream>

#include "constants.hpp"

BBR::BBR(double max_bw, double min_rtt, double lossScale, int fileSize, std::string connectionName)
    : CongestionController(fileSize, connectionName, lossScale) {
    mode_ = Mode::STARTUP;

    cwnd_ = 1;

    max_bandwidth_ = max_bw;
    min_rtt_ = min_rtt;

    target_cwnd_ = 500;
    pacing_gain_ = 2.0;
    gain_cycle_ = 8.0;
    unaknowledged_ = 0;
}

double BBR::lossProbability() {
    if (cwnd_ >= losstresh_) {
        return 100;
    }
    double lossProbability = static_cast<double>(cwnd_) / losstresh_;
    return lossProbability * lossScale_ * 100;
}

void BBR::updateBandwidth(double bandwidth) {
    max_bandwidth_ = bandwidth;
}

double BBR::getSendingRate() {
    double rate = cwnd_ * pacing_gain_;
    if (rate > max_bandwidth_)
        rate = max_bandwidth_;
    return rate;
}

void BBR::onBottleneck() {
    mode_ = Mode::DRAIN;
    cwnd_ *= 0.75;
}

void BBR::onPacketLoss(int count) {
    unaknowledged_ += count;
    mode_ = Mode::PROBE_RTT;
}

void BBR::sendData() {
    switch (mode_) {
    case Mode::STARTUP:
        cwnd_ *= 2;
        if (cwnd_ >= target_cwnd_) {
            mode_ = Mode::DRAIN;
            cwnd_ = target_cwnd_;
        }
        break;
    case Mode::DRAIN:
        cwnd_ -= MSS;
        if (cwnd_ >= target_cwnd_) {
            mode_ = Mode::PROBE_BANDWIDTH;
            cwnd_ = target_cwnd_;
        }
        break;
    case Mode::PROBE_BANDWIDTH:
        cwnd_ += calculateCwndGain();
        break;
    case Mode::PROBE_RTT:
        if (rtt_ > min_rtt_ * 1.25) {
            mode_ = Mode::DRAIN;
            break;
        }
        cwnd_ += 1;
        if (cwnd_ >= target_cwnd_) {
            mode_ = Mode::DRAIN;
            cwnd_ = target_cwnd_;
        }
        mode_ = Mode::PROBE_BANDWIDTH;
        break;
    }
    fileSize_ -= MSS;
}

double BBR::calculateCwndGain() {
    double gain;
    if (cwnd_ < target_cwnd_) {
        double cycle_gain = pow(gain_cycle_, (target_cwnd_ - cwnd_));
        gain = 1.0 + (pacing_gain_ - 1.0) / cycle_gain;
    }
    else {
        double cycle_len = cwnd_ / target_cwnd_;
        gain = 1.0 + pacing_gain_ / cycle_len;
    }
    return gain;
}

void BBR::updateRTT() {
    rtt_ += lossProbability() * 0.01 * (1.12 * min_rtt_ - rtt_);
}

void BBR::log(std::ofstream& dataFile) const {
    dataFile << time_ << " " << cwnd_ << " " << rtt_ << std::endl;
}

void BBR::run() {
    std::ofstream dataFile((PLOT_PATH + connectionName_ + ".txt").c_str());

    while (fileSize_ > 0) {
        if ((rand() % 100 + 1) < lossProbability()) {
            std::cout << connectionName_<< ": Pacekt loss at time " << time_ << ": cwmd = " << cwnd_ << ", RTT = " << rtt_ << std::endl;
            onPacketLoss(1);
        }
        else {
            sendData();
        }

        if (rand() % 5 == 0 && mode_ != Mode::STARTUP) {
            mode_ = Mode::PROBE_RTT;
        }

        log(dataFile);
        updateRTT();
        time_ += rtt_;
    }
    dataFile.close();
}
