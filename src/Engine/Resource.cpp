/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

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

Resource::Resource( const std::string& uri )
	: uri( uri )
{

}

//-----------------------------------//

Resource::~Resource()
{

}


//-----------------------------------//

const std::string& Resource::getPath()
{
	return uri;
}

//-----------------------------------//

void Resource::setPath( const std::string& uri )
{
	this->uri = uri;
}

//-----------------------------------//

} } // end namespaces