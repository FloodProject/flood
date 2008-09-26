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
	namespace renderer {

class Window 
{

public:

	Window (const int width, const int height) 
		: m_width(width), m_height(height) { }
	
	virtual ~Window () { }

	// Factory pattern to create the Window
	static Window& createWindow (const string &title, const int width, const int height, const bool fullscreen);

	// Opens a display window
	virtual bool open (const string &title, const int width, const int height, const bool fullscreen) = 0;

	// Clears the window
	virtual bool clear () = 0;

	// Updates the window
	virtual void update () = 0;

	// Handles the event loop of the window
	virtual bool events () = 0;

	// Sets the title of the window
	virtual void setTitle (const string& title) const = 0;

	// Sets the visibility of the mouse cursor 
	virtual void setCursor (bool state) const = 0;

	// Get window width
	virtual const int getWidth() const { return m_width; }

	// Get window height
	virtual const int getHeight() const { return m_height; }

	// Get the FPS
	virtual const char* getFPS() = 0;

private:

	int m_width, m_height, m_bpp;
	std::string m_title;
	// add fps-related stuff

};

} } // end namespaces