#pragma once

#include "vapor/Window.h"

namespace vapor {
	namespace render {

WindowSettings& Window::getWindowSettings()
{
	return *windowSettings;
}

Window::Window(const string &title, shared_ptr<WindowSettings> windowSettings)
:	windowSettings(windowSettings), title(title)
{
}

void Window::setTitle(const std::string &title) const
{
	
}

Window::~Window()
{

}

} } // end namespaces

		