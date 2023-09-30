#ifndef MASTER_HPP
#define MASTER_HPP

#include <cstdlib>
#include <time.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <array>

#include "ns3/applications-module.h"
#include "ns3/internet-module.h"

#include "my_header.hpp"

using namespace ns3;

const int MAPPERS_COUNT = 3;

class master : public Application
{
public:
    master(uint16_t port, Ipv4InterfaceContainer &ip,
           uint16_t mapper_1_port, uint16_t mapper_2_port, uint16_t mapper_3_port,
           Ipv4InterfaceContainer &mapper_ip);
    virtual ~master();

private:
    virtual void StartApplication(void);
    void HandleRead(Ptr<Socket> socket);

    uint16_t port;
    Ipv4InterfaceContainer ip;
    std::array<uint16_t, MAPPERS_COUNT> mapper_ports;
    Ipv4InterfaceContainer mapper_ip;
    Ptr<Socket> socket;
    std::array<Ptr<Socket>, MAPPERS_COUNT> mapper_sockets;
};


master::master(uint16_t port, Ipv4InterfaceContainer &ip,
               uint16_t mapper_1_port, uint16_t mapper_2_port, uint16_t mapper_3_port,
               Ipv4InterfaceContainer &mapper_ip)
    : port(port),
      ip(ip),
      mapper_ip(mapper_ip)
{
    std::srand(time(0));
    mapper_ports[0] = mapper_1_port;
    mapper_ports[1] = mapper_2_port;
    mapper_ports[2] = mapper_3_port;
}

master::~master()
{
}

void master::StartApplication(void)
{
    socket = Socket::CreateSocket(GetNode(), UdpSocketFactory::GetTypeId());
    InetSocketAddress local = InetSocketAddress(ip.GetAddress(0), port);
    socket->Bind(local);

    mapper_sockets[0] = Socket::CreateSocket(GetNode(), TcpSocketFactory::GetTypeId());
    InetSocketAddress remote_1 = InetSocketAddress(mapper_ip.GetAddress(0), mapper_ports[0]);
    mapper_sockets[0]->Connect(remote_1);

    mapper_sockets[1] = Socket::CreateSocket(GetNode(), TcpSocketFactory::GetTypeId());
    InetSocketAddress remote_2 = InetSocketAddress(mapper_ip.GetAddress(1), mapper_ports[1]);
    mapper_sockets[1]->Connect(remote_2);

    mapper_sockets[2] = Socket::CreateSocket(GetNode(), TcpSocketFactory::GetTypeId());
    InetSocketAddress remote_3 = InetSocketAddress(mapper_ip.GetAddress(2), mapper_ports[2]);
    mapper_sockets[2]->Connect(remote_3);

    socket->SetRecvCallback(MakeCallback(&master::HandleRead, this));
}

void master::HandleRead(Ptr<Socket> socket)
{
    Ptr<Packet> packet;

    while ((packet = socket->Recv()))
    {
        if (packet->GetSize() == 0)
        {
            break;
        }
        MyHeader header;
        packet->RemoveHeader(header);
        Ptr<Packet> packet0 = new Packet();
        Ptr<Packet> packet1 = new Packet();
        Ptr<Packet> packet2 = new Packet();
        packet0->AddHeader(header);
        packet1->AddHeader(header);
        packet2->AddHeader(header);

        mapper_sockets[0]->Send(packet0);
        mapper_sockets[1]->Send(packet1);
        mapper_sockets[2]->Send(packet2);
    }
}


#endif