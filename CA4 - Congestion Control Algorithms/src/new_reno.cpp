#include "new_reno.hpp"

#include <iostream>

#include "constants.hpp"

NewReno::NewReno(int cwnd, int ssthresh, int fileSize) {
    mode_ = Mode::SLOW_START;
    cwnd_ = cwnd;
    ssthresh_ = ssthresh;
    fileSize_ = fileSize;
    rtt_ = RTT;
    losstresh_ = LOSS_TRESH;
    lossScale_ = LOSS_SCALE;
    time_ = 0;
    retransRemain_ = 0;
}

double NewReno::lossProbability() {
    if (cwnd_ >= losstresh_) {
        return 100;
    }
    double lossProbability = static_cast<double>(cwnd_) / losstresh_;
    return lossProbability * lossScale_ * 100;
}

void NewReno::sendData() {
    switch (mode_) {
    case Mode::SLOW_START:
        if (cwnd_ < ssthresh_) {
            cwnd_ *= 2;
        }
        else {
            mode_ = Mode::CONGESTION_AVOIDANCE;
        }
        fileSize_ -= MSS;
        break;
    case Mode::CONGESTION_AVOIDANCE:
        cwnd_ += MSS;
        fileSize_ -= MSS;
        break;
    case Mode::FAST_RECOVERY:
        cwnd_ = cwnd_ / 2;
        ssthresh_ = cwnd_;
        fileSize_ -= MSS;
        mode_ = Mode::FAST_TRANSIMIT;
        break;
    case Mode::FAST_TRANSIMIT:
        retransRemain_--;
        if (retransRemain_ <= 0) {
            mode_ = Mode::CONGESTION_AVOIDANCE;
        }
        break;
    }
}

void NewReno::onRTTUpdate(int newRTT) {
    rtt_ = newRTT;
}

void NewReno::onSelectiveAck(int count) {
    retransRemain_ += count;
    mode_ = Mode::FAST_RECOVERY;
}

void NewReno::log(std::ofstream& dataFile) const {
    dataFile << time_ << " " << cwnd_ << " " << ssthresh_ << std::endl;
}

void NewReno::run() {
    std::ofstream dataFile("data.txt");

    while (fileSize_ > 0) {
        if ((rand() % 100 + 1) < lossProbability()) {
            std::cout << "Pacekt loss at time " << time_ << ": cwmd = " << cwnd_ << ", sstresh = " << ssthresh_ << std::endl;
            onSelectiveAck(1);
        }
        else {
            sendData();
        }

        log(dataFile);
        time_ += rtt_;
    }
    dataFile.close();
}
