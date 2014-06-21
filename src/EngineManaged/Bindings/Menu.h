/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Engine/Window/Menu.h>
#include "StringConverter.h"

namespace Flood
{
    enum struct MenuItemKind;
    ref class Menu;
    ref class MenuBar;
    ref class MenuItem;
}

namespace Flood
{
    /// <summary>
    /// Item kinds for use with menu items and menus.
    /// </summary>
    public enum struct MenuItemKind
    {
        /// <summary> Separator menu item. </summary>
        Separator = 0,
        /// <summary> Normal tool button / menu item. </summary>
        Normal = 1,
        /// <summary> Check (or toggle) tool button / menu item. </summary>
        Check = 2,
        /// <summary> Radio tool button / menu item. </summary>
        Radio = 3
    };

    /// <summary>
    /// Represents a menu item represents an item in a menu.
    /// </summary>
    public ref class MenuItem : ICppInstance
    {
    public:

        property ::MenuItem* NativePtr;
        property System::IntPtr __Instance
        {
            virtual System::IntPtr get();
            virtual void set(System::IntPtr instance);
        }

        MenuItem(::MenuItem* native);
        MenuItem(System::IntPtr native);
        MenuItem();

    private:
        delegate void _SelectedDelegate();
        _SelectedDelegate^ _SelectedDelegateInstance;
        void _SelectedRaise();
        System::Action^ _Selected;
    public:
        event System::Action^ Selected
        {
            void add(System::Action^ evt);
            void remove(System::Action^ evt);
            void raise();
        }
    private:
        delegate void _HighlightedDelegate();
        _HighlightedDelegate^ _HighlightedDelegateInstance;
        void _HighlightedRaise();
        System::Action^ _Highlighted;
    public:
        event System::Action^ Highlighted
        {
            void add(System::Action^ evt);
            void remove(System::Action^ evt);
            void raise();
        }
        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;
    };

    /// <summary>
    /// Represents an OS-specific native window menu, a popup (or pull down) list
    /// of items, one of which may be selected before the menu goes away (clicking
    /// elsewhere dismisses the menu).
    /// </summary>
    public ref class Menu : ICppInstance
    {
    public:

        property ::Menu* NativePtr;
        property System::IntPtr __Instance
        {
            virtual System::IntPtr get();
            virtual void set(System::IntPtr instance);
        }

        Menu(::Menu* native);
        Menu(System::IntPtr native);
        Menu();

        /// <summary>
        /// Adds a menu item.
        /// </summary>
        virtual Flood::MenuItem^ AppendItem(System::String^ item, Flood::MenuItemKind kind);

        /// <summary>
        /// Adds a separator to the end of the menu.
        /// </summary>
        virtual Flood::MenuItem^ AppendSeparatorItem();

        /// <summary>
        /// Deletes the menu item from the menu.
        /// </summary>
        virtual bool DeleteItem(Flood::MenuItem^ item);

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;
    };

    /// <summary>
    /// A menu bar is a series of menus accessible from the top of a window.
    /// </summary>
    public ref class MenuBar : ICppInstance
    {
    public:

        property ::MenuBar* NativePtr;
        property System::IntPtr __Instance
        {
            virtual System::IntPtr get();
            virtual void set(System::IntPtr instance);
        }

        MenuBar(::MenuBar* native);
        MenuBar(System::IntPtr native);
        MenuBar();

        /// <summary>
        /// Adds the item to the end of the menu bar.
        /// </summary>
        virtual bool Append(Flood::Menu^ menu, System::String^ title);

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;
    };
}
