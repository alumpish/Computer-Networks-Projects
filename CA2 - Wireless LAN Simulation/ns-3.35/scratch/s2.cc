#include <cstdlib>
#include <time.h>
#include <stdio.h>
#include <string>
#include <fstream>

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
#include "ns3/network-module.h"
#include "ns3/packet-sink.h"
#include "ns3/error-model.h"
#include "ns3/udp-header.h"
#include "ns3/enum.h"
#include "ns3/event-id.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/traffic-control-module.h"
#include "ns3/flow-monitor-module.h"

using namespace ns3;
using namespace std;
NS_LOG_COMPONENT_DEFINE ("WifiTopology");

int main(int argc, char *argv[])
{
  // Set up command line arguments
  CommandLine cmd;
  cmd.Parse(argc, argv);

  // Set up network topology
  NodeContainer nodes;
  nodes.Create(5); // 5 nodes in total

  // Create a point-to-point link between client and server
  PointToPointHelper clientServerLink;
  clientServerLink.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
  clientServerLink.SetChannelAttribute("Delay", StringValue("2ms"));

  NetDeviceContainer clientServerDevices;
  clientServerDevices = clientServerLink.Install(nodes.Get(0), nodes.Get(1));

  // Create a point-to-point link between server and mapper 1
  PointToPointHelper serverMapperLink1;
  serverMapperLink1.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
  serverMapperLink1.SetChannelAttribute("Delay", StringValue("2ms"));

  NetDeviceContainer serverMapperDevices1;
  serverMapperDevices1 = serverMapperLink1.Install(nodes.Get(1), nodes.Get(2));

  // Create a point-to-point link between server and mapper 2
  PointToPointHelper serverMapperLink2;
  serverMapperLink2.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
  serverMapperLink2.SetChannelAttribute("Delay", StringValue("2ms"));

  NetDeviceContainer serverMapperDevices2;
  serverMapperDevices2 = serverMapperLink2.Install(nodes.Get(1), nodes.Get(3));

  // Create a point-to-point link between server and mapper 3
  PointToPointHelper serverMapperLink3;
  serverMapperLink3.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
  serverMapperLink3.SetChannelAttribute("Delay", StringValue("2ms"));

  NetDeviceContainer serverMapperDevices3;
  serverMapperDevices3 = serverMapperLink3.Install(nodes.Get(1), nodes.Get(4));

  // Create a UDP echo server application
  UdpEchoServerHelper echoServer(9);

  ApplicationContainer serverApps = echoServer.Install(nodes.Get(1));
  serverApps.Start(Seconds(1.0));
  serverApps.Stop(Seconds(10.0));

  // Create a UDP
