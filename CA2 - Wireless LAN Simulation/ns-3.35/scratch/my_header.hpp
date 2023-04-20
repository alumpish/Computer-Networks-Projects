#ifndef MY_HEADER_HPP
#define MY_HEADER_HPP

#include <cstdlib>
#include <time.h>
#include <stdio.h>
#include <string>
#include <fstream>

#include "ns3/udp-header.h"
#include "ns3/yans-wifi-helper.h"

using namespace ns3;

class MyHeader : public Header
{
public:
    MyHeader();
    virtual ~MyHeader();
    void SetData(uint16_t data);
    uint16_t GetData(void) const;
    static TypeId GetTypeId(void);
    virtual TypeId GetInstanceTypeId(void) const;
    virtual void Print(std::ostream &os) const;
    virtual void Serialize(Buffer::Iterator start) const;
    virtual uint32_t Deserialize(Buffer::Iterator start);
    virtual uint32_t GetSerializedSize(void) const;
    void SetIp(Ipv4Address ip);
    Ipv4Address GetIp(void) const;
    void SetPort(uint16_t port);
    uint16_t GetPort(void) const;

private:
    uint16_t m_data;
    Ipv4Address m_ip;
    uint16_t m_port;
};

MyHeader::MyHeader()
{
}

MyHeader::~MyHeader()
{
}

TypeId
MyHeader::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::MyHeader")
                            .SetParent<Header>()
                            .AddConstructor<MyHeader>();
    return tid;
}

TypeId
MyHeader::GetInstanceTypeId(void) const
{
    return GetTypeId();
}

void MyHeader::Print(std::ostream &os) const
{
    os << "data = " << m_data << std::endl;
}

uint32_t
MyHeader::GetSerializedSize(void) const
{
    return 8;
}

void MyHeader::Serialize(Buffer::Iterator start) const
{
    start.WriteHtonU16(m_data);
    start.WriteHtonU32(m_ip.Get());
    start.WriteHtonU16(m_port);
}

uint32_t
MyHeader::Deserialize(Buffer::Iterator start)
{
    m_data = start.ReadNtohU16();
    m_ip.Set(start.ReadNtohU32());
    m_port = start.ReadNtohU16();

    return 8;
}

void MyHeader::SetData(uint16_t data)
{
    m_data = data;
}

uint16_t
MyHeader::GetData(void) const
{
    return m_data;
}

void MyHeader::SetIp(Ipv4Address ip)
{
    m_ip = ip;
}

Ipv4Address
MyHeader::GetIp(void) const
{
    return m_ip;
}

void MyHeader::SetPort(uint16_t port)
{
    m_port = port;
}

uint16_t
MyHeader::GetPort(void) const
{
    return m_port;
}


#endif
