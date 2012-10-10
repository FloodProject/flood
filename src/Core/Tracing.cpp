/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Tracing.h"
#include "Core/Memory.h"
#include "Core/Log.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

static Allocator* gs_TraceAllocator = nullptr;

const int TRACE_QUEUE_SIZE = 16384;

bool g_TraceEnabled = false;

void TraceInitialize()
{
	assert(gs_TraceAllocator == nullptr);

	gs_TraceAllocator = AllocatorCreateBump(
		AllocatorGetHeap(), TRACE_QUEUE_SIZE);

	AllocatorSetGroup(gs_TraceAllocator, "Trace");
}

//-----------------------------------//

NAMESPACE_CORE_END