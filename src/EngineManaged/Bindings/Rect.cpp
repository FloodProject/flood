/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "Rect.h"
#include "ResourceHandle.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::RectSize::RectSize(::RectSize* native)
{
    NativePtr = native;
}

Flood::RectSize::RectSize(System::IntPtr native)
{
    auto __native = (::RectSize*)native.ToPointer();
    NativePtr = __native;
}

int Flood::RectSize::Width::get()
{
    return ((::RectSize*)NativePtr)->width;
}

void Flood::RectSize::Width::set(int value)
{
    ((::RectSize*)NativePtr)->width = value;
}

int Flood::RectSize::Height::get()
{
    return ((::RectSize*)NativePtr)->height;
}

void Flood::RectSize::Height::set(int value)
{
    ((::RectSize*)NativePtr)->height = value;
}

Flood::Rect::Rect(::Rect* native)
{
    NativePtr = native;
}

Flood::Rect::Rect(System::IntPtr native)
{
    auto __native = (::Rect*)native.ToPointer();
    NativePtr = __native;
}

int Flood::Rect::X::get()
{
    return ((::Rect*)NativePtr)->x;
}

void Flood::Rect::X::set(int value)
{
    ((::Rect*)NativePtr)->x = value;
}

int Flood::Rect::Y::get()
{
    return ((::Rect*)NativePtr)->y;
}

void Flood::Rect::Y::set(int value)
{
    ((::Rect*)NativePtr)->y = value;
}

int Flood::Rect::Width::get()
{
    return ((::Rect*)NativePtr)->width;
}

void Flood::Rect::Width::set(int value)
{
    ((::Rect*)NativePtr)->width = value;
}

int Flood::Rect::Height::get()
{
    return ((::Rect*)NativePtr)->height;
}

void Flood::Rect::Height::set(int value)
{
    ((::Rect*)NativePtr)->height = value;
}

Flood::DisjointRectCollection::DisjointRectCollection(::DisjointRectCollection* native)
{
    NativePtr = native;
}

Flood::DisjointRectCollection::DisjointRectCollection(System::IntPtr native)
{
    auto __native = (::DisjointRectCollection*)native.ToPointer();
    NativePtr = __native;
}

bool Flood::DisjointRectCollection::Add(Flood::Rect^ r)
{
    auto &arg0 = *(::Rect*)r->NativePtr;
    auto ret = ((::DisjointRectCollection*)NativePtr)->Add(arg0);
    return ret;
}

void Flood::DisjointRectCollection::Clear()
{
    ((::DisjointRectCollection*)NativePtr)->Clear();
}

bool Flood::DisjointRectCollection::Disjoint(Flood::Rect^ r)
{
    auto &arg0 = *(::Rect*)r->NativePtr;
    auto ret = ((::DisjointRectCollection*)NativePtr)->Disjoint(arg0);
    return ret;
}

bool Flood::DisjointRectCollection::Disjoint(Flood::Rect^ a, Flood::Rect^ b)
{
    auto &arg0 = *(::Rect*)a->NativePtr;
    auto &arg1 = *(::Rect*)b->NativePtr;
    auto ret = ((::DisjointRectCollection*)NativePtr)->Disjoint(arg0, arg1);
    return ret;
}

System::Collections::Generic::List<Flood::Rect^>^ Flood::DisjointRectCollection::Rects::get()
{
    auto _tmpRects = gcnew System::Collections::Generic::List<Flood::Rect^>();
    for(auto _element : ((::DisjointRectCollection*)NativePtr)->rects)
    {
        auto _marshalElement = gcnew Flood::Rect((::Rect*)&_element);
        _tmpRects->Add(_marshalElement);
    }
    return _tmpRects;
}

void Flood::DisjointRectCollection::Rects::set(System::Collections::Generic::List<Flood::Rect^>^ value)
{
    auto _tmpvalue = std::vector<::Rect>();
    for each(Flood::Rect^ _element in value)
    {
        auto _marshalElement = *(::Rect*)_element->NativePtr;
        _tmpvalue.push_back(_marshalElement);
    }
    ((::DisjointRectCollection*)NativePtr)->rects = _tmpvalue;
}

// Performs a lexicographic compare on (rect short side, rect long side).
int Flood::FloodRect::CompareRectShortSide(Flood::Rect^ a, Flood::Rect^ b)
{
    auto &arg0 = *(::Rect*)a->NativePtr;
    auto &arg1 = *(::Rect*)b->NativePtr;
    auto ret = ::CompareRectShortSide(arg0, arg1);
    return ret;
}

// Performs a lexicographic compare on (x, y, width, height).
int Flood::FloodRect::NodeSortCmp(Flood::Rect^ a, Flood::Rect^ b)
{
    auto &arg0 = *(::Rect*)a->NativePtr;
    auto &arg1 = *(::Rect*)b->NativePtr;
    auto ret = ::NodeSortCmp(arg0, arg1);
    return ret;
}

// Returns true if a is contained in b.
bool Flood::FloodRect::IsContainedIn(Flood::Rect^ a, Flood::Rect^ b)
{
    auto &arg0 = *(::Rect*)a->NativePtr;
    auto &arg1 = *(::Rect*)b->NativePtr;
    auto ret = ::IsContainedIn(arg0, arg1);
    return ret;
}

