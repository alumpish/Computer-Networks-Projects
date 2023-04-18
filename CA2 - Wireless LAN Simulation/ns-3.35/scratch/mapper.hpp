#ifndef MAPPER_HPP
#define MAPPER_HPP

#include <cstdlib>
#include <time.h>
#include <stdio.h>
#include <string>
#include <fstream>

#include "ns3/applications-module.h"
#include "ns3/internet-module.h"

#include "my_header.hpp"

using namespace ns3;
using namespace std;


class mapper : public Application
{
public:
    mapper(uint16_t port, Ipv4InterfaceContainer &ip, std::map<int, char> map_set, int size, int i);
    virtual ~mapper();

private:
    virtual void StartApplication(void);
    void HandleRead(Ptr<Socket> socket);
    void HandleAccept(Ptr<Socket> socket, const Address &from);

    uint16_t port;
    Ptr<Socket> socket;
    Ipv4InterfaceContainer ip;
    std::map<int, char> map_set;
    int size;
    int i;
};

mapper::mapper(uint16_t port, Ipv4InterfaceContainer &ip, std::map<int, char> map, int size, int i)
    : port(port),
      ip(ip),
      map_set(map),
      size(size),
      i(i)

{
    std::srand(time(0));
}

mapper::~mapper()
{
}

void mapper::StartApplication(void)
{
    Ptr<Socket> socket = Socket::CreateSocket(GetNode(), TcpSocketFactory::GetTypeId());
    InetSocketAddress local = InetSocketAddress(ip.GetAddress(i), port);
    socket->Bind(local);
    socket->Listen();
    socket->SetAcceptCallback(MakeNullCallback<bool, Ptr<Socket>, const Address &>(), MakeCallback(&mapper::HandleAccept, this));
}

void mapper::HandleRead(Ptr<Socket> socket)
{
    Ptr<Packet> packet;

    while ((packet = socket->Recv()))
    {
        if (packet->GetSize() == 0)
            break;

        MyHeader header;
        packet->RemoveHeader(header);

        uint16_t data = header.GetData();
        Ipv4Address ip = header.GetIp();
        uint16_t port = header.GetPort();
        for (const auto &kv : map_set)
        {
            if (kv.first == data)
            {
                MyHeader header1;
                char new_data = kv.second;
                header1.SetData(new_data);
                Ptr<Packet> packet1 = Create<Packet>(header1.GetSerializedSize());
                packet1->AddHeader(header1);
                Ptr<Socket> socket = Socket::CreateSocket(GetNode(), UdpSocketFactory::GetTypeId());
                InetSocketAddress remote = InetSocketAddress(ip, port);
                socket->Connect(remote);
                socket->Send(packet1);
                socket->Close();
                break;
            }
        }
    }
}

void mapper::HandleAccept(Ptr<Socket> socket, const Address &from)
{
    socket->SetRecvCallback(MakeCallback(&mapper::HandleRead, this));
}

#endif