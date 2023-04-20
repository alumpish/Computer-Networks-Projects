#ifndef UTILS_HPP
#define UTILS_HPP

#include <stdio.h>
#include <time.h>

#include <array>
#include <cstdlib>
#include <fstream>
#include <map>
#include <string>

#include "ns3/core-module.h"
#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/csma-module.h"
#include "ns3/enum.h"
#include "ns3/error-model.h"
#include "ns3/event-id.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/internet-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/mobility-module.h"
#include "ns3/network-module.h"
#include "ns3/packet-sink.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ssid.h"
#include "ns3/traffic-control-module.h"
#include "ns3/udp-header.h"
#include "ns3/yans-wifi-helper.h"

#include "client.hpp"
#include "mapper.hpp"
#include "master.hpp"
#include "my_header.hpp"

using namespace ns3;

std::map<int, char> mapper1_mapping = {{0, 'a'}, {1, 'b'}, {2, 'c'}, {3, 'd'}, {4, 'e'}, {5, 'f'}, {6, 'g'}, {7, 'h'}, {8, 'i'}};
std::map<int, char> mapper2_mapping = {{9, 'j'}, {10, 'k'}, {11, 'l'}, {12, 'm'}, {13, 'n'}, {14, 'o'}, {15, 'p'}, {16, 'q'}, {17, 'r'}};
std::map<int, char> mapper3_mapping = {{18, 's'}, {19, 't'}, {20, 'u'}, {21, 'v'}, {22, 'w'}, {23, 'x'}, {24, 'y'}, {25, 'z'}};

void ThroughputMonitor(FlowMonitorHelper* fmhelper, Ptr<FlowMonitor> flowMon, double em) {
    uint16_t i = 0;

    std::map<FlowId, FlowMonitor::FlowStats> flowStats = flowMon->GetFlowStats();

    Ptr<Ipv4FlowClassifier> classing = DynamicCast<Ipv4FlowClassifier>(fmhelper->GetClassifier());
    for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator stats = flowStats.begin(); stats != flowStats.end(); ++stats) {
        Ipv4FlowClassifier::FiveTuple fiveTuple = classing->FindFlow(stats->first);

        std::cout << "Flow ID			: " << stats->first << " ; " << fiveTuple.sourceAddress << " -----> " << fiveTuple.destinationAddress << std::endl;
        std::cout << "Tx Packets = " << stats->second.txPackets << std::endl;
        std::cout << "Rx Packets = " << stats->second.rxPackets << std::endl;
        std::cout << "Duration		: " << (stats->second.timeLastRxPacket.GetSeconds() - stats->second.timeFirstTxPacket.GetSeconds()) << std::endl;
        std::cout << "Last Received Packet	: " << stats->second.timeLastRxPacket.GetSeconds() << " Seconds" << std::endl;
        std::cout << "Throughput: " << stats->second.rxBytes * 8.0 / (stats->second.timeLastRxPacket.GetSeconds() - stats->second.timeFirstTxPacket.GetSeconds()) / 1024 / 1024 << " Mbps" << std::endl;

        i++;

        std::cout << "---------------------------------------------------------------------------" << std::endl;
    }

    Simulator::Schedule(Seconds(10), &ThroughputMonitor, fmhelper, flowMon, em);
}

void AverageDelayMonitor(FlowMonitorHelper* fmhelper, Ptr<FlowMonitor> flowMon, double em) {
    uint16_t i = 0;

    std::map<FlowId, FlowMonitor::FlowStats> flowStats = flowMon->GetFlowStats();
    Ptr<Ipv4FlowClassifier> classing = DynamicCast<Ipv4FlowClassifier>(fmhelper->GetClassifier());
    for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator stats = flowStats.begin(); stats != flowStats.end(); ++stats) {
        Ipv4FlowClassifier::FiveTuple fiveTuple = classing->FindFlow(stats->first);
        std::cout << "Flow ID			: " << stats->first << " ; " << fiveTuple.sourceAddress << " -----> " << fiveTuple.destinationAddress << std::endl;
        std::cout << "Tx Packets = " << stats->second.txPackets << std::endl;
        std::cout << "Rx Packets = " << stats->second.rxPackets << std::endl;
        std::cout << "Duration		: " << (stats->second.timeLastRxPacket.GetSeconds() - stats->second.timeFirstTxPacket.GetSeconds()) << std::endl;
        std::cout << "Last Received Packet	: " << stats->second.timeLastRxPacket.GetSeconds() << " Seconds" << std::endl;
        std::cout << "Sum of e2e Delay: " << stats->second.delaySum.GetSeconds() << " s" << std::endl;
        std::cout << "Average of e2e Delay: " << stats->second.delaySum.GetSeconds() / stats->second.rxPackets << " s" << std::endl;

        i++;

        std::cout << "---------------------------------------------------------------------------" << std::endl;
    }

    Simulator::Schedule(Seconds(10), &AverageDelayMonitor, fmhelper, flowMon, em);
}

void SetupSim(double duration, double error) {
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

    mobility.SetPositionAllocator("ns3::GridPositionAllocator", "MinX", DoubleValue(0.0), "MinY", DoubleValue(0.0), "DeltaX", DoubleValue(3.0), "DeltaY", DoubleValue(5.0), "GridWidth", UintegerValue(6), "LayoutType", StringValue("RowFirst"));

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

    Ptr<master> masterApp = CreateObject<master>(port, staNodesMasterInterface, port, port, port, staNodesMapperInterface);
    wifiStaNodeMaster.Get(0)->AddApplication(masterApp);
    masterApp->SetStartTime(Seconds(0.0));
    masterApp->SetStopTime(Seconds(duration));

    Ptr<mapper> mapperApp_1 = CreateObject<mapper>(port, staNodesMapperInterface, mapper1_mapping, 9, 1);
    wifiStaNodeMapper.Get(1)->AddApplication(mapperApp_1);
    mapperApp_1->SetStartTime(Seconds(0.0));
    mapperApp_1->SetStopTime(Seconds(duration));

    Ptr<mapper> mapperApp_2 = CreateObject<mapper>(port, staNodesMapperInterface, mapper2_mapping, 9, 0);
    wifiStaNodeMapper.Get(0)->AddApplication(mapperApp_2);
    mapperApp_2->SetStartTime(Seconds(0.0));
    mapperApp_2->SetStopTime(Seconds(duration));

    Ptr<mapper> mapperApp_3 = CreateObject<mapper>(port, staNodesMapperInterface, mapper3_mapping, 8, 2);
    wifiStaNodeMapper.Get(2)->AddApplication(mapperApp_3);
    mapperApp_3->SetStartTime(Seconds(0.0));
    mapperApp_3->SetStopTime(Seconds(duration));
}

#endif