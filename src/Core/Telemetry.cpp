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

NAMESPACE_CORE_BEGIN

//-----------------------------------//

static Allocator* gs_TelemetryAllocator = nullptr;

void TelemetryInitialize()
{
	gs_TelemetryAllocator = AllocatorCreateHeap( AllocatorGetHeap() );
	AllocatorSetGroup(gs_TelemetryAllocator, "Telemetry");
}

void TelemetryCreate()
{
}

void TelemetryDestroy()
{
}

//-----------------------------------//

NAMESPACE_CORE_END