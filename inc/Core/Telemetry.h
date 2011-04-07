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

enum TelemetryEventType
{
	TE_FRAME_BEGIN,
	TE_FRAME_END,

	TE_PROCESS_CONTEXT_SWITCH,

	TE_THREAD_SWITCH,
	TE_THREAD_SWITCH_IDLE,

	TE_TASK_BEGIN,
	TE_TASK_END,
	
	TE_MEMORY_ALLOC,
	TE_MEMORY_FREE,
	
	TE_LOG_INFO,
	TE_LOG_WARN,
	TE_LOG_ERROR,
	TE_LOG_DEBUG,
};

struct TelemetryEvent
{
	TelemetryEventType event;
	int64 time;
};

void TelemetryCreate();
void TelemetryDestroy();

//-----------------------------------//

NAMESPACE_EXTERN_END
