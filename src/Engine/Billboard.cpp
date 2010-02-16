/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/scene/Billboard.h"

namespace vapor { namespace scene {

using namespace vapor::math;

//-----------------------------------//

const std::string& Billboard::type = "Billboard";

//-----------------------------------//

Billboard::Billboard( BillboardType::Enum type )
	: billboardType( type )
{

}

//-----------------------------------//

Billboard::~Billboard( )
{

}

//-----------------------------------//

} } // end namespaces