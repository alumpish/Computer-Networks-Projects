int main(int argc, char *argv[])
{
    // Step 1: Create the nodes
    NodeContainer clientNode, serverNode, mapperNodes;
    clientNode.Create(1);
    serverNode.Create(1);
    mapperNodes.Create(3);

    // Step 2: Set up the point-to-point connections
    PointToPointHelper p2pClientServer, p2pServerMappers;
    p2pClientServer.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    p2pClientServer.SetChannelAttribute("Delay", StringValue("2ms"));
    p2pServerMappers.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    p2pServerMappers.SetChannelAttribute("Delay", StringValue("2ms"));

    NetDeviceContainer clientServerDevices, serverMapperDevices;
    clientServerDevices = p2pClientServer.Install(clientNode.Get(0), serverNode.Get(0));
    for (uint32_t i = 0; i < mapperNodes.GetN(); ++i)
    {
        NetDeviceContainer link = p2pServerMappers.Install(serverNode.Get(0), mapperNodes.Get(i));
        serverMapperDevices.Add(link.Get(1));
    }

    // Step 3: Install the Internet stack
    InternetStackHelper stack;
    stack.Install(clientNode);
    stack.Install(serverNode);
    stack.Install(mapperNodes);

    // Step 4: Assign IP addresses
    Ipv4AddressHelper address;
    address.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer clientServerInterfaces = address.Assign(clientServerDevices);
    address.SetBase("10.1.2.0", "255.255.255.0");
    Ipv4InterfaceContainer serverMapperInterfaces = address.Assign(serverMapperDevices);

    // Step 5: Create UDP and TCP applications
    // Client (UDP)
    uint16_t serverUdpPort = 9;
    UdpClientHelper clientHelper(clientServerInterfaces.GetAddress(1), serverUdpPort);
    clientHelper.SetAttribute("MaxPackets", UintegerValue(1));
    clientHelper.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    clientHelper.SetAttribute("PacketSize", UintegerValue(1024));

    ApplicationContainer clientApp = clientHelper.Install(clientNode.Get(0));
    clientApp.Start(Seconds(1.0));
    clientApp.Stop(Seconds(10.0));

    // Server (UDP + TCP)
    UdpServerHelper serverUdp(serverUdpPort);
    ApplicationContainer serverUdpApp = serverUdp.Install(serverNode.Get(0));
    serverUdpApp.Start(Seconds(0.0));
    serverUdpApp.Stop(Seconds(10.0));

    uint16_t serverTcpPort = 8080;
    TcpServerHelper serverTcp(serverTcpPort);
    ApplicationContainer serverTcpApp = serverTcp.Install(serverNode.Get(0));
    serverTcpApp.Start(Seconds(0.0));
    serverTcpApp.Stop(Seconds(10.0));

    // Mapper nodes (TCP)
    TcpClientHelper mapperTcpHelper(serverMapperInterfaces.GetAddress(0), serverTcpPort);
    mapperTcpHelper.SetAttribute("MaxPackets", UintegerValue(1));
    mapperTcpHelper.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    mapperTcpHelper.SetAttribute("PacketSize", UintegerValue(1024));

    ApplicationContainer mapperTcpApps = mapperTcpHelper.Install(mapperNodes);
    mapperTcpApps.Start(Seconds(2.0));
    mapperTcpApps.Stop(Seconds(10.0));

    // Step 6: Set up the alphabet mapping on each mapper node
    // This can be done by modifying the TcpClientHelper and TcpServerHelper classes
    // to include the alphabet mapping functionality.

    // Step 7: Start the applications and run the simulation
    Ipv4GlobalRoutingHelper::PopulateRoutingTables();
    Simulator::Run();
    Simulator::Destroy();

    return 0;
}