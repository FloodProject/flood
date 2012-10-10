/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/API.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

enum struct TraceEventType : uint16
{
	FrameBegin,
	FrameEnd,
	ProcessContextSwitch,
	ThreadSwitch,
	ThreadSwitchIdle,
	TaskBegin,
	TaskEnd,
	MemoryAlloc,
	MemoryFree,
	LogInfo,
	LogWarn,
	LogError,
	LogDebug
};

typedef uint8 TraceGroup;

struct API_CORE TraceEventData
{
};

struct API_CORE TraceEvent
{
	uint64 time;
	TraceEventType event;
	TraceEventData data;
};

void API_CORE Trace();

void API_CORE TraceSetState(bool enable);
void API_CORE TraceSetMask(uint64 mask);

//-----------------------------------//

NAMESPACE_CORE_END