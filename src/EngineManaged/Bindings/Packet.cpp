/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "Packet.h"
#include "ResourceHandle.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::Packet::Packet(::Packet* native)
{
    NativePtr = native;
}

Flood::Packet::Packet(System::IntPtr native)
{
    auto __native = (::Packet*)native.ToPointer();
    NativePtr = __native;
}

Flood::Packet::Packet(unsigned short id)
{
    auto arg0 = (PacketId)(uint16)id;
    NativePtr = new ::Packet(arg0);
}

Flood::PacketFlags Flood::Packet::GetFlags()
{
    auto ret = ((::Packet*)NativePtr)->getFlags();
    return (Flood::PacketFlags)ret;
}

void Flood::Packet::SetFlags(Flood::PacketFlags v)
{
    auto arg0 = (::PacketFlags)v;
    ((::Packet*)NativePtr)->setFlags(arg0);
}

Flood::PacketFlags Flood::Packet::GetProcessedFlags()
{
    auto ret = ((::Packet*)NativePtr)->getProcessedFlags();
    return (Flood::PacketFlags)ret;
}

void Flood::Packet::SetProcessedFlags(Flood::PacketFlags v)
{
    auto arg0 = (::PacketFlags)v;
    ((::Packet*)NativePtr)->setProcessedFlags(arg0);
}

unsigned short Flood::Packet::GetId()
{
    auto ret = ((::Packet*)NativePtr)->getId();
    return ret;
}

int Flood::Packet::Size()
{
    auto ret = ((::Packet*)NativePtr)->size();
    return ret;
}

void Flood::Packet::Clear()
{
    ((::Packet*)NativePtr)->clear();
}

void Flood::Packet::Write(System::Collections::Generic::List<unsigned char>^ data)
{
    auto _tmpdata = std::vector<::byte>();
    for each(unsigned char _element in data)
    {
        auto _marshalElement = (byte)(uint8)_element;
        _tmpdata.push_back(_marshalElement);
    }
    auto arg0 = _tmpdata;
    ((::Packet*)NativePtr)->write(arg0);
}

System::Collections::Generic::List<unsigned char>^ Flood::Packet::Read()
{
    auto ret = ((::Packet*)NativePtr)->read();
    auto _tmpret = gcnew System::Collections::Generic::List<unsigned char>();
    for(auto _element : ret)
    {
        auto _marshalElement = _element;
        _tmpret->Add(_marshalElement);
    }
    return _tmpret;
}

Flood::Packet^ Flood::Packet::Create(unsigned short id)
{
    auto arg0 = (PacketId)(uint16)id;
    auto ret = ::PacketCreate(arg0);
    return gcnew Flood::Packet((::Packet*)ret.get());
}

