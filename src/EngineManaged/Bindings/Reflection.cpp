/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "Reflection.h"
#include "Serialization.h"
#include "Memory.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::Type::Type(::Type* native)
{
    NativePtr = native;
}

Flood::Type::Type(System::IntPtr native)
{
    NativePtr = (::Type*)native.ToPointer();
}

Flood::Type::Type()
{
    NativePtr = new ::Type();
}

Flood::Type::Type(Flood::TypeKind kind, System::String^ name, unsigned short size)
{
    auto arg0 = (::TypeKind)kind;
    auto _arg1 = clix::marshalString<clix::E_UTF8>(name);
    auto arg1 = _arg1.c_str();
    auto arg2 = (uint16)size;
    NativePtr = new ::Type(arg0, arg1, arg2);
}

Flood::ReflectionDatabase::ReflectionDatabase(::ReflectionDatabase* native)
{
    NativePtr = native;
}

Flood::ReflectionDatabase::ReflectionDatabase(System::IntPtr native)
{
    NativePtr = (::ReflectionDatabase*)native.ToPointer();
}

bool Flood::ReflectionDatabase::RegisterType(Flood::Type^ type)
{
    auto arg0 = NativePtr;
    auto arg1 = type->NativePtr;
    auto ret = ::ReflectionDatabaseRegisterType(arg0, arg1);
    return ret;
}

Flood::Class::Class(::Class* native)
{
    NativePtr = native;
}

Flood::Class::Class(System::IntPtr native)
{
    NativePtr = (::Class*)native.ToPointer();
}

Flood::Class::Class()
{
    NativePtr = new ::Class();
}

bool Flood::Class::Inherits(Flood::Class^ test)
{
    auto arg0 = NativePtr;
    auto arg1 = test->NativePtr;
    auto ret = ::ClassInherits(arg0, arg1);
    return ret;
}

bool Flood::Class::IsAbstract()
{
    auto arg0 = NativePtr;
    auto ret = ::ClassIsAbstract(arg0);
    return ret;
}

void Flood::Class::AddField(Flood::Field^ field)
{
    auto arg0 = NativePtr;
    auto arg1 = field->NativePtr;
    ::ClassAddField(arg0, arg1);
}

Flood::Field^ Flood::Class::GetField(System::String^ name)
{
    auto arg0 = NativePtr;
    auto _arg1 = clix::marshalString<clix::E_UTF8>(name);
    auto arg1 = _arg1.c_str();
    auto ret = ::ClassGetField(arg0, arg1);
    return gcnew Flood::Field((::Field*)ret);
}

Flood::Field^ Flood::Class::GetFieldById(unsigned char id)
{
    auto arg0 = NativePtr;
    auto arg1 = (FieldId)(uint8)id;
    auto ret = ::ClassGetFieldById(arg0, arg1);
    return gcnew Flood::Field((::Field*)ret);
}

System::IntPtr Flood::Class::GetFieldAddress(System::IntPtr _23, Flood::Field^ _24)
{
    auto arg0 = _23.ToPointer();
    auto arg1 = _24->NativePtr;
    auto ret = ::ClassGetFieldAddress(arg0, arg1);
    return IntPtr();
}

System::IntPtr Flood::Class::CreateInstance(Flood::Allocator^ _26)
{
    auto arg0 = NativePtr;
    auto arg1 = _26->NativePtr;
    auto ret = ::ClassCreateInstance(arg0, arg1);
    return IntPtr();
}

unsigned short Flood::Class::CalculateId()
{
    auto arg0 = NativePtr;
    auto ret = ::ClassCalculateId(arg0);
    return ret;
}

Flood::Class^ Flood::Class::GetById(unsigned short id)
{
    auto arg0 = (ClassId)(uint16)id;
    auto ret = ::ClassGetById(arg0);
    return gcnew Flood::Class((::Class*)ret);
}

Flood::Field::Field(::Field* native)
{
    NativePtr = native;
}

Flood::Field::Field(System::IntPtr native)
{
    NativePtr = (::Field*)native.ToPointer();
}

Flood::Field::Field()
{
    NativePtr = new ::Field();
}

bool Flood::Field::HasQualifier(Flood::FieldQualifier _29)
{
    auto arg0 = NativePtr;
    auto arg1 = (::FieldQualifier)_29;
    auto ret = ::FieldHasQualifier(arg0, arg1);
    return ret;
}

void Flood::Field::SetQualifier(Flood::FieldQualifier _31)
{
    auto arg0 = NativePtr;
    auto arg1 = (::FieldQualifier)_31;
    ::FieldSetQualifier(arg0, arg1);
}

void Flood::Field::SetSetter(Flood::FieldSetterFunction^ _33)
{
    auto arg0 = NativePtr;
    auto arg1 = static_cast<::FieldSetterFunction>(System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(_33).ToPointer());
    ::FieldSetSetter(arg0, arg1);
}

Flood::Primitive::Primitive(::Primitive* native)
{
    NativePtr = native;
}

Flood::Primitive::Primitive(System::IntPtr native)
{
    NativePtr = (::Primitive*)native.ToPointer();
}

Flood::Primitive::Primitive()
{
    NativePtr = new ::Primitive();
}

Flood::Primitive::Primitive(Flood::PrimitiveTypeKind kind, System::String^ name, unsigned short size)
{
    auto arg0 = (::PrimitiveTypeKind)kind;
    auto _arg1 = clix::marshalString<clix::E_UTF8>(name);
    auto arg1 = _arg1.c_str();
    auto arg2 = (uint16)size;
    NativePtr = new ::Primitive(arg0, arg1, arg2);
}

Flood::PrimitiveBuiltins^ Flood::Primitive::GetBuiltins()
{
    auto ret = &::PrimitiveGetBuiltins();
    return gcnew Flood::PrimitiveBuiltins((::PrimitiveBuiltins*)ret);
}

Flood::PrimitiveBuiltins::PrimitiveBuiltins(::PrimitiveBuiltins* native)
{
    NativePtr = native;
}

Flood::PrimitiveBuiltins::PrimitiveBuiltins(System::IntPtr native)
{
    NativePtr = (::PrimitiveBuiltins*)native.ToPointer();
}

Flood::PrimitiveBuiltins::PrimitiveBuiltins()
{
    NativePtr = new ::PrimitiveBuiltins();
}

Flood::Enum::Enum(::Enum* native)
{
    NativePtr = native;
}

Flood::Enum::Enum(System::IntPtr native)
{
    NativePtr = (::Enum*)native.ToPointer();
}

void Flood::Enum::AddValue(System::String^ name, int value)
{
    auto arg0 = NativePtr;
    auto _arg1 = clix::marshalString<clix::E_UTF8>(name);
    auto arg1 = _arg1.c_str();
    auto arg2 = (int32)value;
    ::EnumAddValue(arg0, arg1, arg2);
}

int Flood::Enum::GetValue(System::String^ name)
{
    auto arg0 = NativePtr;
    auto _arg1 = clix::marshalString<clix::E_UTF8>(name);
    auto arg1 = _arg1.c_str();
    auto ret = ::EnumGetValue(arg0, arg1);
    return ret;
}

System::String^ Flood::Enum::GetValueName(int value)
{
    auto arg0 = NativePtr;
    auto arg1 = (int32)value;
    auto ret = ::EnumGetValueName(arg0, arg1);
    return clix::marshalString<clix::E_UTF8>(ret);
}

bool Flood::FloodReflection::ReflectionIsPrimitive(Flood::Type^ _11)
{
    auto arg0 = _11->NativePtr;
    auto ret = ::ReflectionIsPrimitive(arg0);
    return ret;
}

bool Flood::FloodReflection::ReflectionIsComposite(Flood::Type^ _12)
{
    auto arg0 = _12->NativePtr;
    auto ret = ::ReflectionIsComposite(arg0);
    return ret;
}

bool Flood::FloodReflection::ReflectionIsEnum(Flood::Type^ _13)
{
    auto arg0 = _13->NativePtr;
    auto ret = ::ReflectionIsEnum(arg0);
    return ret;
}

bool Flood::FloodReflection::ReflectionIsEqual(Flood::Type^ _14, Flood::Type^ _15)
{
    auto arg0 = _14->NativePtr;
    auto arg1 = _15->NativePtr;
    auto ret = ::ReflectionIsEqual(arg0, arg1);
    return ret;
}

Flood::Type^ Flood::FloodReflection::ReflectionFindType(System::String^ _17)
{
    auto _arg0 = clix::marshalString<clix::E_UTF8>(_17);
    auto arg0 = _arg0.c_str();
    auto ret = ::ReflectionFindType(arg0);
    return gcnew Flood::Type((::Type*)ret);
}

bool Flood::FloodReflection::ReflectionRegisterType(Flood::Type^ _18)
{
    auto arg0 = _18->NativePtr;
    auto ret = ::ReflectionRegisterType(arg0);
    return ret;
}

Flood::ReflectionDatabase^ Flood::FloodReflection::ReflectionGetDatabase()
{
    auto ret = &::ReflectionGetDatabase();
    return gcnew Flood::ReflectionDatabase((::ReflectionDatabase*)ret);
}

