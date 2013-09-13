/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Archive.h"
#include "FileWatcher.h"
#include "Memory.h"
#include "Stream.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::Archive::Archive(::Archive* native)
{
    NativePtr = native;
}

Flood::Archive::Archive(System::IntPtr native)
{
    auto __native = (::Archive*)native.ToPointer();
    NativePtr = __native;
}

Flood::Archive::Archive()
{
}

Flood::Archive::Archive(System::String^ path)
{
}

bool Flood::Archive::Open(System::String^ path)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(path);
    auto __ret = ((::Archive*)NativePtr)->open(arg0);
    return __ret;
}

bool Flood::Archive::Close()
{
    auto __ret = ((::Archive*)NativePtr)->close();
    return __ret;
}

Flood::Stream^ Flood::Archive::OpenFile(System::String^ path, Flood::Allocator^ alloc)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(path);
    auto arg1 = (::Allocator*)alloc->NativePtr;
    auto __ret = ((::Archive*)NativePtr)->openFile(arg0, arg1);
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::Stream((::Stream*)__ret);
}

bool Flood::Archive::ExistsFile(System::String^ path)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(path);
    auto __ret = ((::Archive*)NativePtr)->existsFile(arg0);
    return __ret;
}

bool Flood::Archive::ExistsDir(System::String^ path)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(path);
    auto __ret = ((::Archive*)NativePtr)->existsDir(arg0);
    return __ret;
}

void Flood::Archive::EnumerateFiles(System::Collections::Generic::List<System::String^>^ paths)
{
    auto _tmppaths = std::vector<::Path>();
    for each(System::String^ _element in paths)
    {
        auto _marshalElement = clix::marshalString<clix::E_UTF8>(_element);
        _tmppaths.push_back(_marshalElement);
    }
    auto arg0 = _tmppaths;
    ((::Archive*)NativePtr)->enumerateFiles(arg0);
}

void Flood::Archive::EnumerateDirs(System::Collections::Generic::List<System::String^>^ paths)
{
    auto _tmppaths = std::vector<::Path>();
    for each(System::String^ _element in paths)
    {
        auto _marshalElement = clix::marshalString<clix::E_UTF8>(_element);
        _tmppaths.push_back(_marshalElement);
    }
    auto arg0 = _tmppaths;
    ((::Archive*)NativePtr)->enumerateDirs(arg0);
}

bool Flood::Archive::Monitor()
{
    auto __ret = ((::Archive*)NativePtr)->monitor();
    return __ret;
}

System::String^ Flood::Archive::CombinePath(System::String^ filePath)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(filePath);
    auto __ret = ((::Archive*)NativePtr)->combinePath(arg0);
    return clix::marshalString<clix::E_UTF8>(__ret);
}

bool Flood::Archive::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<Archive^>(object);

    if (!obj) return false;
    return Instance == obj->Instance;
}

int Flood::Archive::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::Archive::Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::Archive::Instance::set(System::IntPtr object)
{
    NativePtr = (::Archive*)object.ToPointer();
}

System::String^ Flood::Archive::Path::get()
{
    return clix::marshalString<clix::E_UTF8>(((::Archive*)NativePtr)->path);
}

System::IntPtr Flood::Archive::Userdata::get()
{
    return IntPtr(((::Archive*)NativePtr)->userdata);
}

void Flood::Archive::Userdata::set(System::IntPtr value)
{
    ((::Archive*)NativePtr)->userdata = (void*)value.ToPointer();
}

unsigned int Flood::Archive::WatchId::get()
{
    return ((::Archive*)NativePtr)->watchId;
}

void Flood::Archive::WatchId::set(unsigned int value)
{
    ((::Archive*)NativePtr)->watchId = (::ArchiveWatchId)(::uint32)(::uint32_t)value;
}

bool Flood::Archive::IsValid::get()
{
    return ((::Archive*)NativePtr)->isValid;
}

void Flood::Archive::IsValid::set(bool value)
{
    ((::Archive*)NativePtr)->isValid = value;
}

Flood::ArchiveVirtual::ArchiveVirtual(::ArchiveVirtual* native)
    : Flood::Archive((::Archive*)native)
{
}

Flood::ArchiveVirtual::ArchiveVirtual(System::IntPtr native)
    : Flood::Archive(native)
{
    auto __native = (::ArchiveVirtual*)native.ToPointer();
}

Flood::ArchiveVirtual::ArchiveVirtual()
    : Flood::Archive((::Archive*)nullptr)
{
    NativePtr = new ::ArchiveVirtual();
}

bool Flood::ArchiveVirtual::Open(System::String^ path)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(path);
    auto __ret = ((::ArchiveVirtual*)NativePtr)->open(arg0);
    return __ret;
}

bool Flood::ArchiveVirtual::Close()
{
    auto __ret = ((::ArchiveVirtual*)NativePtr)->close();
    return __ret;
}

Flood::Stream^ Flood::ArchiveVirtual::OpenFile(System::String^ path, Flood::Allocator^ alloc)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(path);
    auto arg1 = (::Allocator*)alloc->NativePtr;
    auto __ret = ((::ArchiveVirtual*)NativePtr)->openFile(arg0, arg1);
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::Stream((::Stream*)__ret);
}

bool Flood::ArchiveVirtual::ExistsFile(System::String^ path)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(path);
    auto __ret = ((::ArchiveVirtual*)NativePtr)->existsFile(arg0);
    return __ret;
}

bool Flood::ArchiveVirtual::ExistsDir(System::String^ path)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(path);
    auto __ret = ((::ArchiveVirtual*)NativePtr)->existsDir(arg0);
    return __ret;
}

void Flood::ArchiveVirtual::EnumerateFiles(System::Collections::Generic::List<System::String^>^ paths)
{
    auto _tmppaths = std::vector<::Path>();
    for each(System::String^ _element in paths)
    {
        auto _marshalElement = clix::marshalString<clix::E_UTF8>(_element);
        _tmppaths.push_back(_marshalElement);
    }
    auto arg0 = _tmppaths;
    ((::ArchiveVirtual*)NativePtr)->enumerateFiles(arg0);
}

void Flood::ArchiveVirtual::EnumerateDirs(System::Collections::Generic::List<System::String^>^ paths)
{
    auto _tmppaths = std::vector<::Path>();
    for each(System::String^ _element in paths)
    {
        auto _marshalElement = clix::marshalString<clix::E_UTF8>(_element);
        _tmppaths.push_back(_marshalElement);
    }
    auto arg0 = _tmppaths;
    ((::ArchiveVirtual*)NativePtr)->enumerateDirs(arg0);
}

bool Flood::ArchiveVirtual::Monitor()
{
    auto __ret = ((::ArchiveVirtual*)NativePtr)->monitor();
    return __ret;
}

bool Flood::ArchiveVirtual::Mount(Flood::Archive^ mount, System::String^ mountPath)
{
    auto arg0 = (::Archive*)mount->NativePtr;
    auto arg1 = clix::marshalString<clix::E_UTF8>(mountPath);
    auto __ret = ((::ArchiveVirtual*)NativePtr)->mount(arg0, arg1);
    return __ret;
}

void Flood::ArchiveVirtual::MountDirectories(System::String^ dirPath, Flood::Allocator^ alloc)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(dirPath);
    auto arg1 = (::Allocator*)alloc->NativePtr;
    ((::ArchiveVirtual*)NativePtr)->mountDirectories(arg0, arg1);
}

bool Flood::ArchiveVirtual::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<ArchiveVirtual^>(object);

    if (!obj) return false;
    return Instance == obj->Instance;
}

int Flood::ArchiveVirtual::GetHashCode()
{
    return (int)NativePtr;
}

System::Collections::Generic::List<Flood::Archive^>^ Flood::ArchiveVirtual::Mounts::get()
{
    auto _tmpMounts = gcnew System::Collections::Generic::List<Flood::Archive^>();
    for(auto _element : ((::ArchiveVirtual*)NativePtr)->mounts)
    {
        auto _marshalElement = gcnew Flood::Archive((::Archive*)_element);
        _tmpMounts->Add(_marshalElement);
    }
    return _tmpMounts;
}

void Flood::ArchiveVirtual::Mounts::set(System::Collections::Generic::List<Flood::Archive^>^ value)
{
    auto _tmpvalue = std::vector<::Archive*>();
    for each(Flood::Archive^ _element in value)
    {
        auto _marshalElement = (::Archive*)_element->NativePtr;
        _tmpvalue.push_back(_marshalElement);
    }
    ((::ArchiveVirtual*)NativePtr)->mounts = _tmpvalue;
}

Flood::ArchiveDirectory::ArchiveDirectory(::ArchiveDirectory* native)
    : Flood::Archive((::Archive*)native)
{
}

Flood::ArchiveDirectory::ArchiveDirectory(System::IntPtr native)
    : Flood::Archive(native)
{
    auto __native = (::ArchiveDirectory*)native.ToPointer();
}

Flood::ArchiveDirectory::ArchiveDirectory(System::String^ _0)
    : Flood::Archive((::Archive*)nullptr)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(_0);
    NativePtr = new ::ArchiveDirectory(arg0);
}

bool Flood::ArchiveDirectory::Open(System::String^ path)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(path);
    auto __ret = ((::ArchiveDirectory*)NativePtr)->open(arg0);
    return __ret;
}

bool Flood::ArchiveDirectory::Close()
{
    auto __ret = ((::ArchiveDirectory*)NativePtr)->close();
    return __ret;
}

Flood::Stream^ Flood::ArchiveDirectory::OpenFile(System::String^ path, Flood::Allocator^ alloc)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(path);
    auto arg1 = (::Allocator*)alloc->NativePtr;
    auto __ret = ((::ArchiveDirectory*)NativePtr)->openFile(arg0, arg1);
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::Stream((::Stream*)__ret);
}

bool Flood::ArchiveDirectory::ExistsFile(System::String^ path)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(path);
    auto __ret = ((::ArchiveDirectory*)NativePtr)->existsFile(arg0);
    return __ret;
}

bool Flood::ArchiveDirectory::ExistsDir(System::String^ path)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(path);
    auto __ret = ((::ArchiveDirectory*)NativePtr)->existsDir(arg0);
    return __ret;
}

void Flood::ArchiveDirectory::EnumerateFiles(System::Collections::Generic::List<System::String^>^ paths)
{
    auto _tmppaths = std::vector<::Path>();
    for each(System::String^ _element in paths)
    {
        auto _marshalElement = clix::marshalString<clix::E_UTF8>(_element);
        _tmppaths.push_back(_marshalElement);
    }
    auto arg0 = _tmppaths;
    ((::ArchiveDirectory*)NativePtr)->enumerateFiles(arg0);
}

void Flood::ArchiveDirectory::EnumerateDirs(System::Collections::Generic::List<System::String^>^ paths)
{
    auto _tmppaths = std::vector<::Path>();
    for each(System::String^ _element in paths)
    {
        auto _marshalElement = clix::marshalString<clix::E_UTF8>(_element);
        _tmppaths.push_back(_marshalElement);
    }
    auto arg0 = _tmppaths;
    ((::ArchiveDirectory*)NativePtr)->enumerateDirs(arg0);
}

bool Flood::ArchiveDirectory::Monitor()
{
    auto __ret = ((::ArchiveDirectory*)NativePtr)->monitor();
    return __ret;
}

bool Flood::ArchiveDirectory::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<ArchiveDirectory^>(object);

    if (!obj) return false;
    return Instance == obj->Instance;
}

int Flood::ArchiveDirectory::GetHashCode()
{
    return (int)NativePtr;
}

Flood::ArchiveZip::ArchiveZip(::ArchiveZip* native)
    : Flood::Archive((::Archive*)native)
{
}

Flood::ArchiveZip::ArchiveZip(System::IntPtr native)
    : Flood::Archive(native)
{
    auto __native = (::ArchiveZip*)native.ToPointer();
}

Flood::ArchiveZip::ArchiveZip(System::String^ path)
    : Flood::Archive((::Archive*)nullptr)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(path);
    NativePtr = new ::ArchiveZip(arg0);
}

bool Flood::ArchiveZip::Open(System::String^ path)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(path);
    auto __ret = ((::ArchiveZip*)NativePtr)->open(arg0);
    return __ret;
}

bool Flood::ArchiveZip::Close()
{
    auto __ret = ((::ArchiveZip*)NativePtr)->close();
    return __ret;
}

Flood::Stream^ Flood::ArchiveZip::OpenFile(System::String^ path, Flood::Allocator^ alloc)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(path);
    auto arg1 = (::Allocator*)alloc->NativePtr;
    auto __ret = ((::ArchiveZip*)NativePtr)->openFile(arg0, arg1);
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::Stream((::Stream*)__ret);
}

bool Flood::ArchiveZip::ExistsFile(System::String^ path)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(path);
    auto __ret = ((::ArchiveZip*)NativePtr)->existsFile(arg0);
    return __ret;
}

bool Flood::ArchiveZip::ExistsDir(System::String^ path)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(path);
    auto __ret = ((::ArchiveZip*)NativePtr)->existsDir(arg0);
    return __ret;
}

void Flood::ArchiveZip::EnumerateFiles(System::Collections::Generic::List<System::String^>^ paths)
{
    auto _tmppaths = std::vector<::Path>();
    for each(System::String^ _element in paths)
    {
        auto _marshalElement = clix::marshalString<clix::E_UTF8>(_element);
        _tmppaths.push_back(_marshalElement);
    }
    auto arg0 = _tmppaths;
    ((::ArchiveZip*)NativePtr)->enumerateFiles(arg0);
}

void Flood::ArchiveZip::EnumerateDirs(System::Collections::Generic::List<System::String^>^ paths)
{
    auto _tmppaths = std::vector<::Path>();
    for each(System::String^ _element in paths)
    {
        auto _marshalElement = clix::marshalString<clix::E_UTF8>(_element);
        _tmppaths.push_back(_marshalElement);
    }
    auto arg0 = _tmppaths;
    ((::ArchiveZip*)NativePtr)->enumerateDirs(arg0);
}

bool Flood::ArchiveZip::Monitor()
{
    auto __ret = ((::ArchiveZip*)NativePtr)->monitor();
    return __ret;
}

bool Flood::ArchiveZip::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<ArchiveZip^>(object);

    if (!obj) return false;
    return Instance == obj->Instance;
}

int Flood::ArchiveZip::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::ArchiveZip::Handle::get()
{
    return IntPtr(((::ArchiveZip*)NativePtr)->handle);
}

void Flood::ArchiveZip::Handle::set(System::IntPtr value)
{
    ((::ArchiveZip*)NativePtr)->handle = (void*)value.ToPointer();
}

