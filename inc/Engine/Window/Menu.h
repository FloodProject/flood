/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Engine/API.h"
#include "Core/String.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

/**
 * Item kinds for use with menu items and menus.
 */
enum class MenuItemKind
{
    /// Separator menu item.
    Separator,
    /// Normal tool button / menu item.
    Normal,
    /// Check (or toggle) tool button / menu item.
    Check,
    /// Radio tool button / menu item.
    Radio
};

/**
 * Represents a menu item represents an item in a menu.
 */
class API_ENGINE MenuItem
{
public:

    virtual ~MenuItem();

    /// Event fired when the item is selected.
    Event0<> onSelected;

    /// Event fired when the item is highlighted.
    Event0<> onHighlighted;
};

/**
 * Represents an OS-specific native window menu, a popup (or pull down)
 * list of items, one of which may be selected before the menu goes away
 * (clicking elsewhere dismisses the menu).
 */
class API_ENGINE Menu
{
public:

    virtual ~Menu();

    /// Adds a menu item.
    virtual MenuItem* appendItem(const String& item, MenuItemKind kind) = 0;

    /// Adds a separator to the end of the menu.
    virtual MenuItem* appendSeparatorItem() = 0;

    /// Deletes the menu item from the menu.
    virtual bool deleteItem(MenuItem* item) = 0;
};

/**
 * A menu bar is a series of menus accessible from the top of a window.
 */
class API_ENGINE MenuBar
{
public:

    virtual ~MenuBar();

    /// Adds the item to the end of the menu bar.
    virtual bool append(Menu* menu, const String& title) = 0;
};

//-----------------------------------//

NAMESPACE_ENGINE_END