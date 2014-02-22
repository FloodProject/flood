/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "WxPlatform.h"
#include "Engine/Window/Menu.h"

class wxMenuBar;
class wxMenu;
class wxMenuItem;

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class WxMenu : public Menu
{
public:

    WxMenu(wxMenu* menu);
    MenuItem* appendItem(const String& item, MenuItemKind kind) override;
    MenuItem* appendSeparatorItem() override;
    bool deleteItem(MenuItem* item) override;

    void onMenu(wxCommandEvent& event);
    void onMenuHighlight(wxMenuEvent& event);

    wxMenu* menu;
};

class WxMenuItem : public MenuItem
{
public:

    WxMenuItem(wxMenuItem* item);

    wxMenuItem* item;
};

class WxMenuBar : public MenuBar
{
public:

    WxMenuBar(wxMenuBar* menuBar);
    bool append(Menu* menu, const String& title) override;

    wxMenuBar* menuBar;
};

//-----------------------------------//

NAMESPACE_EDITOR_END