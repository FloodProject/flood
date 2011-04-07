/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Core/Telemetry.h"
#include "Core/Memory.h"
#include "Core/Log.h"

NAMESPACE_BEGIN

//-----------------------------------//

static Allocator* AllocatorTelemetry = AllocatorCreateHeap( AllocatorGetHeap(), "Telemetry" );

void TelemetryCreate()
{
}

void TelemetryDestroy()
{
}

//-----------------------------------//

NAMESPACE_END