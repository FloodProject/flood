/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Packet.h"

using namespace System;
using namespace System::Runtime::InteropServices;

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
    auto arg0 = (::PacketId)(::uint16)(::uint16_t)id;
    NativePtr = new ::Packet(arg0);
}

int Flood::Packet::Size()
{
    auto __ret = ((::Packet*)NativePtr)->size();
    return __ret;
}

void Flood::Packet::Clear()
{
    ((::Packet*)NativePtr)->clear();
}

void Flood::Packet::Write(System::Collections::Generic::List<unsigned char>^ data)
{
    auto _tmpdata = Vector<::byte>();
    for each(unsigned char _element in data)
    {
        auto _marshalElement = (::byte)(::uint8)(::uint8_t)_element;
        _tmpdata.Push(_marshalElement);
    }
    auto arg0 = _tmpdata;
    ((::Packet*)NativePtr)->write(arg0);
}

System::Collections::Generic::List<unsigned char>^ Flood::Packet::Read()
{
    auto __ret = ((::Packet*)NativePtr)->read();
    auto _tmp__ret = gcnew System::Collections::Generic::List<unsigned char>();
    for(auto _element : __ret)
    {
        auto _marshalElement = _element;
        _tmp__ret->Add(_marshalElement);
    }
    return _tmp__ret;
}

bool Flood::Packet::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<Packet^>(object);

    if (!obj) return false;
    return __Instance == obj->__Instance;
}

int Flood::Packet::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::Packet::__Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::Packet::__Instance::set(System::IntPtr object)
{
    NativePtr = (::Packet*)object.ToPointer();
}

Flood::PacketFlags Flood::Packet::Flags::get()
{
    auto __ret = ((::Packet*)NativePtr)->getFlags();
    return (Flood::PacketFlags)__ret;
}

void Flood::Packet::Flags::set(Flood::PacketFlags value)
{
    auto v = value;
    auto arg0 = (::PacketFlags)v;
    ((::Packet*)NativePtr)->setFlags(arg0);
}

Flood::PacketFlags Flood::Packet::ProcessedFlags::get()
{
    auto __ret = ((::Packet*)NativePtr)->getProcessedFlags();
    return (Flood::PacketFlags)__ret;
}

void Flood::Packet::ProcessedFlags::set(Flood::PacketFlags value)
{
    auto v = value;
    auto arg0 = (::PacketFlags)v;
    ((::Packet*)NativePtr)->setProcessedFlags(arg0);
}

unsigned short Flood::Packet::Id::get()
{
    auto __ret = ((::Packet*)NativePtr)->getId();
    return __ret;
}

