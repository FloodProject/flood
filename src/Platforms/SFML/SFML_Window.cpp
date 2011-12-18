/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "SFML_Window.h"

#ifdef ENABLE_WINDOW_SFML

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

SFML_Window::SFML_Window(const WindowSettings& settings)
	: Window(settings)
	, cursorState(true)
	, flags(0)
{

}

//-----------------------------------//

SFML_Window::~SFML_Window()
{
	window.Close();
}

//-----------------------------------//

bool SFML_Window::open()
{
	contextSettings.DepthBits = settings.depthBits; 
	contextSettings.StencilBits = settings.stencilBits;
	contextSettings.AntialiasingLevel = settings.antialiasLevel;
	
	videoMode = sf::VideoMode::GetDesktopMode();

	if( settings.fullScreen )
	{
		flags |= sf::Style::Fullscreen;
	}
	else
	{
		videoMode.Width = settings.width; 
		videoMode.Height = settings.height;
		videoMode.BitsPerPixel = settings.bitsPerPixel;
		
		if( !videoMode.IsValid() )
		{
			LogError( "Video mode not supportted." );
			return false;
		}
		
		flags = sf::Style::Close;
	}
	
	createWindow();

	contextSettings = window.GetSettings();
	
	assert( contextSettings.DepthBits >= 0 && contextSettings.DepthBits <= 32 );
	settings.depthBits = contextSettings.DepthBits;
	
	assert( contextSettings.StencilBits >= 0 && contextSettings.StencilBits <= 32 );
	settings.stencilBits = contextSettings.StencilBits;
	
	assert( contextSettings.AntialiasingLevel >= 0 && contextSettings.AntialiasingLevel <= 32 );
	settings.antialiasLevel = contextSettings.AntialiasingLevel;
	
	//window.EnableKeyRepeat( false );
	
	return true;
}

//-----------------------------------//

void SFML_Window::createWindow()
{
	if(settings.handle)
	{
		sf::WindowHandle handle = (sf::WindowHandle) settings.handle;
		window.Create( handle, contextSettings );
	}
	else
	{
		window.Create( videoMode, settings.title, flags, contextSettings );
	}

	context = AllocateThis(RenderContext);
	context->mainTarget = this;
}

//-----------------------------------//

void SFML_Window::update() 
{
	window.Display();
}

//-----------------------------------//

void SFML_Window::show( bool hide ) 
{
	window.Show( hide );
}

//-----------------------------------//

void SFML_Window::makeCurrent()
{
	window.SetActive(); 
}

//-----------------------------------//

bool SFML_Window::pumpEvents()
{
	sf::Event event;

	while( window.PollEvent(event) )
	{
	switch( event.Type )
	{
	case sf::Event::Closed:
		handleWindowClose();
		return false;

	case sf::Event::Resized:
		processResize( event );
		break;

	case sf::Event::LostFocus:
		handleWindowFocus( true );
		break;

	case sf::Event::GainedFocus:
		handleWindowFocus( false );
		break;

	default:
		input.processSFMLEvent( event );
		break;
	} }

	return true;
}

//-----------------------------------//

void SFML_Window::setTitle(const String& title) 
{
	settings.title = title;
	createWindow();
	
	LogInfo( "Changing window title to '%s'", title.c_str() );
}

//-----------------------------------//

void SFML_Window::setCursorVisible(bool state)
{
	cursorState = state;
	window.ShowMouseCursor(state);
}

//-----------------------------------//

bool SFML_Window::isCursorVisible() const
{
	return cursorState;
}

//-----------------------------------//

void SFML_Window::setCursorPosition( int x, int y )
{
	sf::Mouse::SetPosition( sf::Vector2i(x, y), window );
}

//-----------------------------------//

Vector2i SFML_Window::getCursorPosition() const
{
	sf::Vector2i pos = sf::Mouse::GetPosition();
	return Vector2i(pos.x, pos.y);
}

//-----------------------------------//

void SFML_Window::processResize(sf::Event event)
{
	settings.height = (uint16) event.Size.Height;
	settings.width = (uint16) event.Size.Width;

	handleWindowResize();
}

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif