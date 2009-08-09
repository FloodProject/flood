/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/resources/Resource.h"

namespace vapor {
	namespace resources {

//-----------------------------------//

namespace ResourceGroup
{
	const std::string getString(Enum e) 
	{
		switch(e) 
		{
		case General:
			return "General";
		case Images:
			return "Images";
		case Meshes:
			return "Meshes";
		case Fonts:
			return "Fonts";
		case Shaders:
			return "Shaders";
		case Audio:
			return "Audio";
		default:
			return "Undefined";
		}
	}
}

//-----------------------------------//

Resource::Resource()
{

}

//-----------------------------------//

Resource::~Resource()
{

}


//-----------------------------------//

} } // end namespaces