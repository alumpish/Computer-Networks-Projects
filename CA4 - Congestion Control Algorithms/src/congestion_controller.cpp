#include "congestion_controller.hpp"

#include "constants.hpp"

CongestionController::CongestionController(int fileSize, std::string connectionName, double lossScale)
    : fileSize_(fileSize), connectionName_(connectionName), lossScale_(lossScale) {
    rtt_ = RTT;
    losstresh_ = LOSS_TRESH;
    time_ = 0;
    retransRemain_ = 0;
}
