/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/Profiler.h"

namespace vapor {

//-----------------------------------//

Profiler::Profiler(const std::string& name)
	: name( name )
{ }

//-----------------------------------//

Profiler::~Profiler()
{
	ulong time = static_cast<ulong>(timer.getElapsedTime());

	debug( "%s: took %lu ms / %.5lf µs.", name.c_str(), 
		time*1000, timer.getElapsedTime() );
};

//-----------------------------------//

} // end namespace 