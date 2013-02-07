/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Core/Serialization.h>
#include "Reflection.h"
#include "Memory.h"
#include "Vector.h"
#include "Color.h"
#include "Quaternion.h"

namespace Flood
{
    enum struct ReflectionWalkType : unsigned char;
    ref class Object;
    ref class ReflectionContext;
    ref class Class;
    ref class Allocator;
    ref class Field;
    ref class Type;
    enum struct TypeKind : unsigned char;
    enum struct FieldQualifier : unsigned short;
    ref class Primitive;
    enum struct PrimitiveTypeKind : unsigned char;
    ref class PrimitiveBuiltins;
    ref class Enum;
    ref class ValueContext;
    ref class ReflectionHandleContext;
    ref class HandleManager;
    ref class Serializer;
    ref class Stream;

    public enum struct ReflectionWalkType : unsigned char
    {
        Begin = 0,
        End = 1,
        Element = 2,
        ElementBegin = 3,
        ElementEnd = 4
    };

    public delegate unsigned int ReflectionDeserializeHandleFn(System::String^);

    public delegate Flood::Object^ SerializerLoadFunction(Flood::Serializer^);

    public delegate bool SerializerSaveFunction(Flood::Serializer^, Flood::Object^);

    public ref class ValueContext
    {
    public:
        property ::ValueContext* NativePtr;

        ValueContext(::ValueContext* native);
        ValueContext(System::IntPtr native);
        property bool B;
        property char I8;
        property unsigned char U8;
        property short I16;
        property unsigned short U16;
        property int I32;
        property unsigned int U32;
        property long long I64;
        property unsigned long long U64;
        property float F32;
        property Flood::Vector3P V;
        property Flood::ColorP C;
        property Flood::QuaternionP Q;
        property System::String^ S;
        property System::String^ Cs;
    };

    public ref class ReflectionHandleContext
    {
    public:
        property ::ReflectionHandleContext* NativePtr;

        ReflectionHandleContext(::ReflectionHandleContext* native);
        ReflectionHandleContext(System::IntPtr native);
        property Flood::Class^ Type;
        property Flood::ReflectionDeserializeHandleFn^ Deserialize;
    };

    public ref class ReflectionContext
    {
    public:
        property ::ReflectionContext* NativePtr;

        ReflectionContext(::ReflectionContext* native);
        ReflectionContext(System::IntPtr native);
        property bool Loading;
        property System::IntPtr UserData;
        property Flood::Class^ ObjectClass;
        property Flood::Type^ Type;
        property Flood::Primitive^ Primitive;
        property Flood::Class^ Composite;
        property Flood::Enum^ Enume;
        property Flood::ValueContext^ ValueContext;
        property Flood::Field^ Field;
        property System::IntPtr Address;
        property System::IntPtr ElementAddress;
        property unsigned int ArraySize;
        ReflectionContext();
    };

    public ref class Serializer
    {
    public:
        property ::Serializer* NativePtr;

        Serializer(::Serializer* native);
        Serializer(System::IntPtr native);
        property Flood::Allocator^ Alloc;
        property Flood::Stream^ Stream;
        property Flood::ReflectionContext^ SerializeContext;
        property Flood::ReflectionContext^ DeserializeContext;
        Serializer();
        void Destroy();
    };

    public ref class FloodSerialization
    {
    public:
        static void ReflectionWalkComposite(Flood::ReflectionContext^ _112);
        static void ReflectionWalkCompositeField(Flood::ReflectionContext^ _113);
    };
}
