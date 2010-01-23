/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/Profiler.h"

namespace vapor {

//-----------------------------------//

Profiler::Profiler(const std::string& name)
	: name( name )
{

}

//-----------------------------------//

Profiler::~Profiler()
{
	debug("%s: took %lu ms.", name.c_str(), static_cast<ulong>(time.getElapsedTime()*1000) );
};

//-----------------------------------//

} // end namespace 