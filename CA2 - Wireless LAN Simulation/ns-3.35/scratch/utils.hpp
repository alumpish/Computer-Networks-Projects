#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstdlib>
#include <time.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <array>
#include <map>

#include "ns3/flow-monitor-helper.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/traffic-control-module.h"
#include "ns3/flow-monitor-module.h"

#include "my_header.hpp"

using namespace ns3;
using namespace std;


void ThroughputMonitor(FlowMonitorHelper *fmhelper, Ptr<FlowMonitor> flowMon, double em)
{
    uint16_t i = 0;

    std::map<FlowId, FlowMonitor::FlowStats> flowStats = flowMon->GetFlowStats();

    Ptr<Ipv4FlowClassifier> classing = DynamicCast<Ipv4FlowClassifier>(fmhelper->GetClassifier());
    for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator stats = flowStats.begin(); stats != flowStats.end(); ++stats)
    {
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

void AverageDelayMonitor(FlowMonitorHelper *fmhelper, Ptr<FlowMonitor> flowMon, double em)
{
    uint16_t i = 0;

    std::map<FlowId, FlowMonitor::FlowStats> flowStats = flowMon->GetFlowStats();
    Ptr<Ipv4FlowClassifier> classing = DynamicCast<Ipv4FlowClassifier>(fmhelper->GetClassifier());
    for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator stats = flowStats.begin(); stats != flowStats.end(); ++stats)
    {
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

static void GenerateTraffic(Ptr<Socket> socket, Ipv4InterfaceContainer ip, uint16_t port, uint16_t data)
{
    Ptr<Packet> packet = new Packet();
    MyHeader m;
    m.SetData(data);
    m.SetIp(ip.GetAddress(0));
    m.SetPort(port);

    packet->AddHeader(m);
    packet->Print(std::cout);
    socket->Send(packet);

    Simulator::Schedule(Seconds(0.1), &GenerateTraffic, socket, ip, port, rand() % 26);
}

#endif