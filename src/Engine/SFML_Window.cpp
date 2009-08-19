/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/render/SFML_Window.h"

#ifdef VAPOR_WINDOWING_SFML

namespace vapor {
	namespace render {

//-----------------------------------//

SFMLWindow::SFMLWindow(Settings& settings)
	: Window(settings), flags(0)
{
	if( !open() ) 
		exit(1);
}

//-----------------------------------//

SFMLWindow::~SFMLWindow()
{

}

//-----------------------------------//

bool SFMLWindow::open()
{
		sfmlSettings.DepthBits         = settings.getDepthBits(); 
		sfmlSettings.StencilBits       = settings.getStencilBits();
		sfmlSettings.AntialiasingLevel = settings.getAALevel();
		
		if( settings.isFullscreen() )
		{
			vMode = sf::VideoMode::GetMode(0);
			flags |= sf::Style::Fullscreen;
		}
		else
		{
			vMode.Width = settings.getWidth(); 
			vMode.Height = settings.getHeight();
			vMode.BitsPerPixel = settings.getBpp();
			
			if( !vMode.IsValid() )
			{
				error( "render::window::sfml", "Video mode not supportted." );
				return false;
			}
			
			flags |= sf::Style::Resize | sf::Style::Close;			
		}
		
		createWindow();

		sfmlSettings = window.GetSettings();
		settings.setDepthBits( sfmlSettings.DepthBits );
		settings.setStencilBits( sfmlSettings.StencilBits );
		settings.setAALevel( sfmlSettings.AntialiasingLevel );
		
		return true;	
}
//-----------------------------------//

void SFMLWindow::createWindow()
{
	if(settings.getCustomHandle())
	{
		window.Create(settings.getCustomHandle(), sfmlSettings);	
	}
	else
	{		
		window.Create(vMode, settings.getTitle(), flags, sfmlSettings);
	}
}
//-----------------------------------//

void SFMLWindow::update() 
{
	window.Display();
}

//-----------------------------------//

void SFMLWindow::makeCurrent()
{
	window.SetActive(); 
}

//-----------------------------------//

bool SFMLWindow::pumpEvents()
{
	sf::Event Event;
	
	while( window.GetEvent(Event) )
	{
		switch( Event.Type )
		{
			case sf::Event::Closed:
				return false;
			case sf::Event::Resized:
				processResize(Event);
				break;
			case sf::Event::LostFocus:
				//still to figure out 
				break;	
			case sf::Event::GainedFocus:
				//still to figure out
				break;	
			case sf::Event::TextEntered:
			case sf::Event::KeyPressed:
			case sf::Event::KeyReleased:
			case sf::Event::MouseWheelMoved:
			case sf::Event::MouseButtonPressed:
			case sf::Event::MouseButtonReleased:
			case sf::Event::MouseMoved:
			case sf::Event::MouseEntered:
			case sf::Event::MouseLeft:
			case sf::Event::JoyButtonPressed:
			case sf::Event::JoyButtonReleased:
			case sf::Event::JoyMoved:
				//call input mananger to process this type of event
				break;
		}

	}

	return true;
}

//-----------------------------------//

void SFMLWindow::setTitle(const std::string& title) 
{
	settings.setTitle(title);
	createWindow();
	
	info( "render::window::sfml", "Changing window title to '%s'", title.c_str() );
}

//-----------------------------------//

void SFMLWindow::setCursor(bool state)
{
	window.ShowMouseCursor(state);
}

//-----------------------------------//

void SFMLWindow::processResize(sf::Event event)
{
	settings.setHeight(event.Size.Height);
	settings.setWidth(event.Size.Width);
}

//-----------------------------------//

} } // end namespaces

#endif