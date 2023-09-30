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

class mapper : public Application
{
public:
    mapper(uint16_t id, uint16_t port, Ipv4InterfaceContainer &ip, std::map<int, char> map_set, int size, int i);
    virtual ~mapper();

private:
    virtual void StartApplication(void);
    void HandleRead(Ptr<Socket> socket);
    void HandleAccept(Ptr<Socket> socket, const Address &from);

    uint16_t id;
    uint16_t port;
    Ptr<Socket> server_socket;
    Ipv4InterfaceContainer ip;
    Ptr<Socket> client_socket;
    std::map<int, char> map_set;
    int size;
    int i;
};

mapper::mapper(uint16_t id, uint16_t port, Ipv4InterfaceContainer &ip, std::map<int, char> map, int size, int i)
    : id(id),
      port(port),
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
    server_socket = Socket::CreateSocket(GetNode(), TcpSocketFactory::GetTypeId());
    InetSocketAddress local = InetSocketAddress(ip.GetAddress(i), port);
    server_socket->Bind(local);
    server_socket->Listen();
    server_socket->SetAcceptCallback(MakeNullCallback<bool, Ptr<Socket>, const Address &>(), MakeCallback(&mapper::HandleAccept, this));

    client_socket = Socket::CreateSocket(GetNode(), UdpSocketFactory::GetTypeId());
}

void mapper::HandleRead(Ptr<Socket> socket)
{
    Ptr<Packet> g_packet;

    while ((g_packet = socket->Recv()))
    {
        if (g_packet->GetSize() == 0)
            break;

        MyHeader g_header;
        g_packet->RemoveHeader(g_header);

        uint16_t data = g_header.GetData();
        Ipv4Address ip = g_header.GetIp();
        uint16_t port = g_header.GetPort();

        for (const auto &kv : map_set)
        {
            if (kv.first == data)
            {
                MyHeader s_header;
                char new_data = kv.second;
                s_header.SetData(new_data);
                s_header.SetId(id);
                Ptr<Packet> s_packet = Create<Packet>(s_header.GetSerializedSize());
                s_packet->AddHeader(s_header);
                
                InetSocketAddress remote = InetSocketAddress (ip, port);
                client_socket->SendTo(s_packet, 0, remote);
                // socket->Close();
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