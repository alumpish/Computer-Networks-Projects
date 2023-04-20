#include "ns3/core-module.h"

#include "utils.hpp"
#include "consts.hpp"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("WifiTopology");


int main(int argc, char *argv[])
{
    double error = 0.000001;
    bool verbose = true;
    double duration = 60.0;
    bool tracing = false;

    srand(time(NULL));

    CommandLine cmd(__FILE__);
    cmd.AddValue("verbose", "Tell echo applications to log if true", verbose);
    cmd.AddValue("tracing", "Enable pcap tracing", tracing);

    cmd.Parse(argc, argv);

    if (verbose)
    {
        LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
        LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);
    }

    SetupSim(duration, error);

    NS_LOG_INFO("Run Simulation");

    Ptr<FlowMonitor> flowMonitor;
    FlowMonitorHelper flowHelper;
    flowMonitor = flowHelper.InstallAll();

    ThroughputAndAvgDelayMonitor(&flowHelper, flowMonitor, error);

    Simulator::Stop(Seconds(duration));
    Simulator::Run();

    return 0;
}