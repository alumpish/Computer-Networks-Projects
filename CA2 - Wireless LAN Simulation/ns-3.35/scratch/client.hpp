#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <cstdlib>
#include <time.h>
#include <stdio.h>
#include <string>
#include <fstream>

#include "ns3/applications-module.h"
#include "ns3/internet-module.h"


#include "my_header.hpp"
#include "utils.hpp"

using namespace ns3;
using namespace std;


class client : public Application
{
public:
    client(uint16_t port, Ipv4InterfaceContainer &ip, uint16_t master_port, Ipv4InterfaceContainer &master_ip);
    virtual ~client();
    uint16_t GetPort(void);
    Ipv4InterfaceContainer GetIp(void);
    Ptr<Socket> GetSocket();

private:
    virtual void StartApplication(void);
    void HandleRead(Ptr<Socket> socket);

    uint16_t port;
    Ipv4InterfaceContainer ip;
    Ptr<Socket> socketUDPmaster, socketUDPmapper;
    uint16_t master_port;
    Ipv4InterfaceContainer master_ip;
};


client::client(uint16_t port, Ipv4InterfaceContainer &ip, uint16_t master_port, Ipv4InterfaceContainer &master_ip)
    : port(port),
      ip(ip),
      master_port(master_port),
      master_ip(master_ip)
{
    std::srand(time(0));
}

client::~client()
{
}

Ipv4InterfaceContainer client::GetIp()
{
    return ip;
}

uint16_t client::GetPort()
{
    return port;
}

Ptr<Socket> client::GetSocket()
{
    return socketUDPmaster;
}

void client::StartApplication(void)
{
    // Create socket for UDP between client and master
    Ptr<Socket> sock = Socket::CreateSocket(GetNode(), UdpSocketFactory::GetTypeId());
    InetSocketAddress sockAddr(master_ip.GetAddress(0), master_port);
    sock->Connect(sockAddr);

    // Create socket for UDP between client and mapper
    Ptr<Socket> sock2 = Socket::CreateSocket(GetNode(), UdpSocketFactory::GetTypeId());
    InetSocketAddress sockAddr2(ip.GetAddress(0), port);
    sock2->Bind(sockAddr2);
    sock2->SetRecvCallback(MakeCallback(&client::HandleRead, this));

    GenerateTraffic(sock, ip, port, 0);
}

void client::HandleRead(Ptr<Socket> socket)
{
    Ptr<Packet> packet;
    while ((packet = socket->Recv()))
    {
        cout << "hiiiiiiiiiiii\n";
        if (packet->GetSize() == 0)
        {
            break;
        }
        MyHeader m;
        packet->RemoveHeader(m);
        std::cout << "Received packet from " << m.GetIp() << ":" << m.GetPort() << " with data " << static_cast<char>(m.GetData()) << std::endl;
    }
}

#endif