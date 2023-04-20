#include <cstdlib>
#include <time.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <array>

#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/ssid.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/error-model.h"
#include "ns3/ipv4-global-routing-helper.h"


#include "master.hpp"
#include "client.hpp"
#include "mapper.hpp"
#include "my_header.hpp"
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

    NodeContainer wifiStaNodeClient;
    wifiStaNodeClient.Create(1);

    NodeContainer wifiStaNodeMaster;
    wifiStaNodeMaster.Create(1);

    NodeContainer wifiStaNodeMapper;
    wifiStaNodeMapper.Create(3);

    YansWifiChannelHelper channel = YansWifiChannelHelper::Default();

    YansWifiPhyHelper phy;
    phy.SetChannel(channel.Create());

    WifiHelper wifi;
    wifi.SetRemoteStationManager("ns3::AarfWifiManager");

    WifiMacHelper mac;
    Ssid ssid = Ssid("ns-3-ssid");

    mac.SetType("ns3::StaWifiMac", "Ssid", SsidValue(ssid), "ActiveProbing", BooleanValue(false));
    NetDeviceContainer staDeviceClient;
    staDeviceClient = wifi.Install(phy, mac, wifiStaNodeClient);

    mac.SetType("ns3::StaWifiMac", "Ssid", SsidValue(ssid), "ActiveProbing", BooleanValue(false));
    NetDeviceContainer staDeviceMapper;
    staDeviceMapper = wifi.Install(phy, mac, wifiStaNodeMapper);

    mac.SetType("ns3::ApWifiMac", "Ssid", SsidValue(ssid));
    NetDeviceContainer staDeviceMaster;
    staDeviceMaster = wifi.Install(phy, mac, wifiStaNodeMaster);

    Ptr<RateErrorModel> em = CreateObject<RateErrorModel>();
    em->SetAttribute("ErrorRate", DoubleValue(error));
    phy.SetErrorRateModel("ns3::YansErrorRateModel");

    MobilityHelper mobility;

    mobility.SetPositionAllocator("ns3::GridPositionAllocator",
                                  "MinX", DoubleValue(0.0),
                                  "MinY", DoubleValue(0.0),
                                  "DeltaX", DoubleValue(3.0),
                                  "DeltaY", DoubleValue(5.0),
                                  "GridWidth", UintegerValue(6),
                                  "LayoutType", StringValue("RowFirst"));

    mobility.SetMobilityModel("ns3::RandomWalk2dMobilityModel", "Bounds", RectangleValue(Rectangle(-50, 50, -50, 50)));
    mobility.Install(wifiStaNodeClient);

    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(wifiStaNodeMaster);

    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(wifiStaNodeMapper);

    InternetStackHelper stack;
    stack.Install(wifiStaNodeClient);
    stack.Install(wifiStaNodeMaster);
    stack.Install(wifiStaNodeMapper);

    Ipv4AddressHelper address;

    Ipv4InterfaceContainer staNodeClientInterface;
    Ipv4InterfaceContainer staNodesMasterInterface;
    Ipv4InterfaceContainer staNodesMapperInterface;

    address.SetBase("10.1.3.0", "255.255.255.0");

    staNodeClientInterface = address.Assign(staDeviceClient);
    staNodesMasterInterface = address.Assign(staDeviceMaster);
    staNodesMapperInterface = address.Assign(staDeviceMapper);

    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    uint16_t port = 1102;

    Ptr<client> clientApp = CreateObject<client>(port, staNodeClientInterface, port, staNodesMasterInterface);
    wifiStaNodeClient.Get(0)->AddApplication(clientApp);
    clientApp->SetStartTime(Seconds(0.0));
    clientApp->SetStopTime(Seconds(duration));

    Ptr<master> masterApp = CreateObject<master>(port, staNodesMasterInterface, port, port, port,
                                                 staNodesMapperInterface);
    wifiStaNodeMaster.Get(0)->AddApplication(masterApp);
    masterApp->SetStartTime(Seconds(0.0));
    masterApp->SetStopTime(Seconds(duration));

    Ptr<mapper> mapperApp_1 = CreateObject<mapper>(port, staNodesMapperInterface, kMapper1, 9, 1);
    wifiStaNodeMapper.Get(1)->AddApplication(mapperApp_1);
    mapperApp_1->SetStartTime(Seconds(0.0));
    mapperApp_1->SetStopTime(Seconds(duration));

    Ptr<mapper> mapperApp_2 = CreateObject<mapper>(port, staNodesMapperInterface, kMapper2, 9, 0);
    wifiStaNodeMapper.Get(0)->AddApplication(mapperApp_2);
    mapperApp_2->SetStartTime(Seconds(0.0));
    mapperApp_2->SetStopTime(Seconds(duration));

    Ptr<mapper> mapperApp_3 = CreateObject<mapper>(port, staNodesMapperInterface, kMapper3, 8, 2);
    wifiStaNodeMapper.Get(2)->AddApplication(mapperApp_3);
    mapperApp_3->SetStartTime(Seconds(0.0));
    mapperApp_3->SetStopTime(Seconds(duration));

    NS_LOG_INFO("Run Simulation");

    Ptr<FlowMonitor> flowMonitor;
    FlowMonitorHelper flowHelper;
    flowMonitor = flowHelper.InstallAll();

    Simulator::Stop(Seconds(duration));
    Simulator::Run();

    return 0;
}