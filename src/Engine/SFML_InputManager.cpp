/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/CompileOptions.h"

#ifdef VAPOR_INPUT_SFML

#include "vapor/input/SFML_InputManager.h"

namespace vapor {
	namespace input {

//-----------------------------------//

SFML_InputManager::SFML_InputManager()
{

}

//-----------------------------------//

SFML_InputManager::~SFML_InputManager()
{

}

//-----------------------------------//

void SFML_InputManager::processEvent( const sf::Event& event )
{

}

//-----------------------------------//

} } // end namespaces

#endif