/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Core/Reflection.h>
#include "Memory.h"

namespace Flood
{
    enum struct TypeAttributeKind : unsigned char;
    enum struct TypeKind : unsigned char;
    enum struct ReflectionWalkType : unsigned char;
    enum struct FieldQualifier : unsigned short;
    enum struct PrimitiveTypeKind : unsigned char;
    ref class Type;
    ref class ReflectionContext;
    ref class ReflectionDatabase;
    ref class Class;
    ref class Allocator;
    ref class Object;
    ref class Field;
    ref class PrimitiveBuiltins;
    ref class Primitive;
    ref class Enum;

    /// <summary>
    /// Types can have metadata attached to them in the form of attributes.
    /// </summary>
    public enum struct TypeAttributeKind : unsigned char
    {
        String = 0,
        Integer = 1,
        KeyValue = 2,
        Flags = 3
    };

    /// <summary>
    /// Provides types with a fast RTTI (Runtime Type Information) system that will
    /// be used for fast dynamic type checking, reflection and serialization.
    /// </summary>
    public enum struct TypeKind : unsigned char
    {
        Primitive = 0,
        Composite = 1,
        Enumeration = 2
    };

    public enum struct FieldQualifier : unsigned short
    {
        Array = 1,
        Map = 2,
        Set = 4,
        Handle = 8,
        RawPointer = 16,
        SharedPointer = 32,
        RefPointer = 64,
        ReadOnly = 128,
        NoSerialize = 256
    };

    public enum struct PrimitiveTypeKind : unsigned char
    {
        Bool = 0,
        Int8 = 1,
        Uint8 = 2,
        Int16 = 3,
        Uint16 = 4,
        Int32 = 5,
        Uint32 = 6,
        Int64 = 7,
        Uint64 = 8,
        Float = 9,
        String = 10,
        Color = 11,
        Vector3 = 12,
        Quaternion = 13
    };

    public delegate void ReflectionWalkFunction(Flood::ReflectionContext^, Flood::ReflectionWalkType);

    public delegate System::IntPtr ClassCreateFunction(Flood::Allocator^);

    public delegate void FieldSetterFunction(System::IntPtr object, System::IntPtr value);

    public delegate System::IntPtr FieldResizeFunction(System::IntPtr object, unsigned int size);

    public ref class Type
    {
    public:
        property ::Type* NativePtr;

        Type(::Type* native);
        Type(System::IntPtr native);
        Type();
        Type(Flood::TypeKind kind, System::String^ name, unsigned short size);
        property Flood::TypeKind Kind;
        property System::String^ Name;
        property unsigned short Size;
    };

    public ref class ReflectionDatabase
    {
    public:
        property ::ReflectionDatabase* NativePtr;

        ReflectionDatabase(::ReflectionDatabase* native);
        ReflectionDatabase(System::IntPtr native);
        bool RegisterType(Flood::Type^ type);
    };

    /// <summary>
    /// This class provides types with a fast RTTI (Runtime Type Information)
    /// system that will be used for fast dynamic type checking and reflection.
    /// Main use is for serialization and property introspection.
    /// </summary>
    public ref class Class : Type
    {
    public:
        Class(::Class* native);
        Class(System::IntPtr native);
        Class();
        property unsigned short Id;
        property Flood::Class^ Parent;
        property Flood::ClassCreateFunction^ Create_fn;
        bool Inherits(Flood::Class^ test);
        bool IsAbstract();
        void AddField(Flood::Field^ field);
        Flood::Field^ GetField(System::String^ name);
        Flood::Field^ GetFieldById(unsigned char id);
        System::IntPtr CreateInstance(Flood::Allocator^ _26);
        unsigned short CalculateId();
        static System::IntPtr GetFieldAddress(System::IntPtr _23, Flood::Field^ _24);
        static Flood::Class^ GetById(unsigned short id);
    };

    public ref class Field
    {
    public:
        property ::Field* NativePtr;

        Field(::Field* native);
        Field(System::IntPtr native);
        Field();
        property Flood::Type^ Type;
        property unsigned char Id;
        property System::String^ Name;
        property unsigned short Size;
        property unsigned short Offset;
        property unsigned short Pointer_size;
        property Flood::FieldQualifier Qualifiers;
        property Flood::FieldSetterFunction^ Setter;
        property Flood::FieldResizeFunction^ Resize;
        bool HasQualifier(Flood::FieldQualifier _29);
        void SetQualifier(Flood::FieldQualifier _31);
        void SetSetter(Flood::FieldSetterFunction^ _33);
    };

    public ref class Primitive : Type
    {
    public:
        Primitive(::Primitive* native);
        Primitive(System::IntPtr native);
        Primitive();
        Primitive(Flood::PrimitiveTypeKind kind, System::String^ name, unsigned short size);
        property Flood::PrimitiveTypeKind Kind;
        static Flood::PrimitiveBuiltins^ GetBuiltins();
    };

    public ref class PrimitiveBuiltins
    {
    public:
        property ::PrimitiveBuiltins* NativePtr;

        PrimitiveBuiltins(::PrimitiveBuiltins* native);
        PrimitiveBuiltins(System::IntPtr native);
        PrimitiveBuiltins();
        property Flood::Primitive^ P_bool;
        property Flood::Primitive^ P_int8;
        property Flood::Primitive^ P_uint8;
        property Flood::Primitive^ P_int16;
        property Flood::Primitive^ P_uint16;
        property Flood::Primitive^ P_int32;
        property Flood::Primitive^ P_uint32;
        property Flood::Primitive^ P_int64;
        property Flood::Primitive^ P_uint64;
        property Flood::Primitive^ P_float;
        property Flood::Primitive^ P_double;
        property Flood::Primitive^ P_string;
        property Flood::Primitive^ P_Vector3;
        property Flood::Primitive^ P_Color;
        property Flood::Primitive^ P_Quaternion;
    };

    public ref class Enum : Type
    {
    public:
        Enum(::Enum* native);
        Enum(System::IntPtr native);
        property Flood::PrimitiveTypeKind Backing;
        void AddValue(System::String^ name, int value);
        int GetValue(System::String^ name);
        System::String^ GetValueName(int value);
    };

    public ref class FloodReflection
    {
    public:
        static bool ReflectionIsPrimitive(Flood::Type^ _11);
        static bool ReflectionIsComposite(Flood::Type^ _12);
        static bool ReflectionIsEnum(Flood::Type^ _13);
        static bool ReflectionIsEqual(Flood::Type^ _14, Flood::Type^ _15);
        static Flood::Type^ ReflectionFindType(System::String^ _17);
        static bool ReflectionRegisterType(Flood::Type^ _18);
        static Flood::ReflectionDatabase^ ReflectionGetDatabase();
    };
}
