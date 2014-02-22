/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Menu.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::MenuItem::MenuItem(::MenuItem* native)
{
    NativePtr = native;
}

Flood::MenuItem::MenuItem(System::IntPtr native)
{
    auto __native = (::MenuItem*)native.ToPointer();
    NativePtr = __native;
}

Flood::MenuItem::MenuItem()
{
    NativePtr = new ::MenuItem();
}

bool Flood::MenuItem::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<MenuItem^>(object);

    if (!obj) return false;
    return __Instance == obj->__Instance;
}

int Flood::MenuItem::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::MenuItem::__Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::MenuItem::__Instance::set(System::IntPtr object)
{
    NativePtr = (::MenuItem*)object.ToPointer();
}
void Flood::MenuItem::Selected::add(System::Action^ evt)
{
    if (!_SelectedDelegateInstance)
    {
        _SelectedDelegateInstance = gcnew _SelectedDelegate(this, &Flood::MenuItem::_SelectedRaise);
        auto _fptr = (void (*)())Marshal::GetFunctionPointerForDelegate(_SelectedDelegateInstance).ToPointer();
        ((::MenuItem*)NativePtr)->onSelected.Connect(_fptr);
    }
    _Selected = static_cast<System::Action^>(System::Delegate::Combine(_Selected, evt));
}

void Flood::MenuItem::Selected::remove(System::Action^ evt)
{
    _Selected = static_cast<System::Action^>(System::Delegate::Remove(_Selected, evt));
}

void Flood::MenuItem::Selected::raise()
{
    _Selected();
}

void Flood::MenuItem::_SelectedRaise()
{
    Selected::raise();
}

void Flood::MenuItem::Highlighted::add(System::Action^ evt)
{
    if (!_HighlightedDelegateInstance)
    {
        _HighlightedDelegateInstance = gcnew _HighlightedDelegate(this, &Flood::MenuItem::_HighlightedRaise);
        auto _fptr = (void (*)())Marshal::GetFunctionPointerForDelegate(_HighlightedDelegateInstance).ToPointer();
        ((::MenuItem*)NativePtr)->onHighlighted.Connect(_fptr);
    }
    _Highlighted = static_cast<System::Action^>(System::Delegate::Combine(_Highlighted, evt));
}

void Flood::MenuItem::Highlighted::remove(System::Action^ evt)
{
    _Highlighted = static_cast<System::Action^>(System::Delegate::Remove(_Highlighted, evt));
}

void Flood::MenuItem::Highlighted::raise()
{
    _Highlighted();
}

void Flood::MenuItem::_HighlightedRaise()
{
    Highlighted::raise();
}

Flood::Menu::Menu(::Menu* native)
{
    NativePtr = native;
}

Flood::Menu::Menu(System::IntPtr native)
{
    auto __native = (::Menu*)native.ToPointer();
    NativePtr = __native;
}

Flood::MenuItem^ Flood::Menu::AppendItem(System::String^ item, Flood::MenuItemKind kind)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(item);
    auto arg1 = (::MenuItemKind)kind;
    auto __ret = ((::Menu*)NativePtr)->appendItem(arg0, arg1);
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::MenuItem((::MenuItem*)__ret);
}

Flood::MenuItem^ Flood::Menu::AppendSeparatorItem()
{
    auto __ret = ((::Menu*)NativePtr)->appendSeparatorItem();
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::MenuItem((::MenuItem*)__ret);
}

bool Flood::Menu::DeleteItem(Flood::MenuItem^ item)
{
    auto arg0 = (::MenuItem*)item->NativePtr;
    auto __ret = ((::Menu*)NativePtr)->deleteItem(arg0);
    return __ret;
}

Flood::Menu::Menu()
{
}

bool Flood::Menu::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<Menu^>(object);

    if (!obj) return false;
    return __Instance == obj->__Instance;
}

int Flood::Menu::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::Menu::__Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::Menu::__Instance::set(System::IntPtr object)
{
    NativePtr = (::Menu*)object.ToPointer();
}
Flood::MenuBar::MenuBar(::MenuBar* native)
{
    NativePtr = native;
}

Flood::MenuBar::MenuBar(System::IntPtr native)
{
    auto __native = (::MenuBar*)native.ToPointer();
    NativePtr = __native;
}

bool Flood::MenuBar::Append(Flood::Menu^ menu, System::String^ title)
{
    auto arg0 = (::Menu*)menu->NativePtr;
    auto arg1 = clix::marshalString<clix::E_UTF8>(title);
    auto __ret = ((::MenuBar*)NativePtr)->append(arg0, arg1);
    return __ret;
}

Flood::MenuBar::MenuBar()
{
}

bool Flood::MenuBar::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<MenuBar^>(object);

    if (!obj) return false;
    return __Instance == obj->__Instance;
}

int Flood::MenuBar::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::MenuBar::__Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::MenuBar::__Instance::set(System::IntPtr object)
{
    NativePtr = (::MenuBar*)object.ToPointer();
}
