/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//// 
//// vaporEngine by triton
////
//// License: fuckGNU License
////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>

using namespace std;

namespace vapor {
	namespace render {

class WindowSettings
{
public:

	WindowSettings(const int width, const int height, const int bpp, const bool fullscreen)
		: width(width), height(height), fullscreen(fullscreen) { }

	int width, height, bpp;
	bool fullscreen;
};

class Window 
{

public:

	Window (const string& title, WindowSettings windowSettings);
	virtual ~Window ();

	// Updates the window
	virtual void update () = 0;

	// Handles the event loop of the window
	virtual bool pump () = 0;

	// Sets the title of the window
	virtual void setTitle (const string& title) const;

	// Sets the visibility of the mouse cursor 
	virtual void setCursor (bool state) const = 0;

private:

	WindowSettings& windowSettings;
	string title;
};

} } // end namespaces