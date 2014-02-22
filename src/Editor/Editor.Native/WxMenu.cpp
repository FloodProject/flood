/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "WxMenu.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

WxMenu::WxMenu(wxMenu* menu)
    : menu(menu)
{
    assert(menu && "Expected a valid menu instance");
    menu->SetClientData(this);

    menu->Bind(wxEVT_MENU, &WxMenu::onMenu, this);
    menu->Bind(wxEVT_MENU_HIGHLIGHT, &WxMenu::onMenuHighlight, this);
}

struct WxMenuItemRefData : public wxObjectRefData
{
    WxMenuItemRefData(WxMenuItem* item) : menuItem(item) {}
    WxMenuItem* menuItem;
};

void WxMenu::onMenu(wxCommandEvent& event)
{
    auto menuItem = menu->FindItem(event.GetId());
    if (!menuItem) return;

    auto menuItemData = (WxMenuItemRefData*) menuItem->GetRefData();
    assert(menuItemData && "Expected a valid menu item data instance");

    auto wxMenuItem = ((WxMenuItem*)menuItemData->menuItem);
    assert(wxMenuItem && "Expected a valid wxWidgets menu item instance");

    wxMenuItem->onSelected();
}

void WxMenu::onMenuHighlight(wxMenuEvent& event)
{
    //((WxMenu*)event.GetMenu())->onHighlighted();
}

static wxItemKind ConvertMenuItemKindToWx(MenuItemKind kind)
{
    switch(kind)
    {
    case MenuItemKind::Separator: return wxITEM_SEPARATOR;
    case MenuItemKind::Normal: return wxITEM_NORMAL;
    case MenuItemKind::Check:  return wxITEM_CHECK;
    case MenuItemKind::Radio:  return wxITEM_RADIO;
    default: fld_unreachable("Unknown menu item kind");
    }
}

MenuItem* WxMenu::appendItem(const String& item, MenuItemKind kind)
{
    auto itemKind = ConvertMenuItemKindToWx(kind);
    auto menuItem = menu->Append(wxID_ANY, item, wxEmptyString, itemKind);

    auto wxMenuItem = new WxMenuItem(menuItem);
    menuItem->SetRefData(new WxMenuItemRefData(wxMenuItem));

    return wxMenuItem;
}

MenuItem* WxMenu::appendSeparatorItem()
{
    auto menuItem = menu->AppendSeparator();

    auto wxMenuItem = new WxMenuItem(menuItem);
    menuItem->SetRefData(new WxMenuItemRefData(wxMenuItem));

    return wxMenuItem;
}

bool WxMenu::deleteItem(MenuItem* item)
{
    return 0;
}

//-----------------------------------//

WxMenuItem::WxMenuItem(wxMenuItem* item)
    : item(item)
{
    assert(item && "Expected a valid menu item instance");
}

//-----------------------------------//

WxMenuBar::WxMenuBar(wxMenuBar* menuBar)
    : menuBar(menuBar)
{
    assert(menuBar && "Expected a valid menu bar instance");
}

bool WxMenuBar::append(Menu* menu, const String& title)
{
	auto wxMenu = (WxMenu*)menu;
	return menuBar->Append(wxMenu->menu, title);
}

//-----------------------------------//

NAMESPACE_EDITOR_END