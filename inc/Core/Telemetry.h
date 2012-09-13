/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

NAMESPACE_EXTERN_BEGIN

//-----------------------------------//

struct TelemetryEventType
{
	enum Enum
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
};

struct TelemetryEvent
{
	TelemetryEventType::Enum event;
	int64 time;
};

void TelemetryCreate();
void TelemetryDestroy();

//-----------------------------------//

NAMESPACE_EXTERN_END
