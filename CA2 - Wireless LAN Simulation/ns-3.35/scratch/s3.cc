// Create three new Mapper nodes 
NodeContainer wifiStaNodeMapper1, wifiStaNodeMapper2, wifiStaNodeMapper3; 
wifiStaNodeMapper1.Create(1); 
wifiStaNodeMapper2.Create(1); 
wifiStaNodeMapper3.Create(1); 
 
// Configure WiFi network for the three new Mapper nodes 
WifiMacHelper mac; 
mac.SetType("ns3::StaWifiMac", "Ssid", SsidValue(ssid), "ActiveProbing", BooleanValue(false)); 
NetDeviceContainer staDeviceMapper1, staDeviceMapper2, staDeviceMapper3; 
staDeviceMapper1 = wifi.Install(phy, mac, wifiStaNodeMapper1); 
staDeviceMapper2 = wifi.Install(phy, mac, wifiStaNodeMapper2); 
staDeviceMapper3 = wifi.Install(phy, mac, wifiStaNodeMapper3); 
 
// Assign IP addresses to the three new Mapper nodes 
Ipv4InterfaceContainer mapper1Interface, mapper2Interface, mapper3Interface; 
address.SetBase("10.1.4.0", "255.255.255.0"); 
mapper1Interface = address.Assign(staDeviceMapper1); 
address.SetBase("10.1.5.0", "255.255.255.0"); 
mapper2Interface = address.Assign(staDeviceMapper2); 
address.SetBase("10.1.6.0", "255.255.255.0"); 
mapper3Interface = address.Assign(staDeviceMapper3); 
 
// Add Mapper applications to the three new Mapper nodes 
Ptr<Mapper> mapperApp1 = CreateObject<Mapper> (port, staNodesMasterInterface, mapper1Interface); 
wifiStaNodeMapper1.Get (0)->AddApplication (mapperApp1); 
mapperApp1->SetStartTime (Seconds (0.0)); 
mapperApp1->SetStopTime (Seconds (duration)); 
 
Ptr<Mapper> mapperApp2 = CreateObject<Mapper> (port, staNodesMasterInterface, mapper2Interface); 
wifiStaNodeMapper2.Get (0)->AddApplication (mapperApp2); 
mapperApp2->SetStartTime (Seconds (0.0)); 
mapperApp2->SetStopTime (Seconds (duration)); 
 
Ptr<Mapper> mapperApp3 = CreateObject<Mapper> (port, staNodesMasterInterface, mapper3Interface); 
wifiStaNodeMapper3.Get (0)->AddApplication (mapperApp3); 
mapperApp3->SetStartTime (Seconds (0.0)); 
mapperApp3->SetStopTime (Seconds (duration)); 
 
// Send data to the new Mapper nodes 
clientApp->AddNode(staNodeMapper1.Get(0)->GetObject<Ipv4>(), port); 
clientApp->AddNode(staNodeMapper2.Get(0)->GetObject<Ipv4>(), port); 
clientApp->AddNode(staNodeMapper3.Get(0)->GetObject<Ipv4>(), port);