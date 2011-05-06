/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
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
