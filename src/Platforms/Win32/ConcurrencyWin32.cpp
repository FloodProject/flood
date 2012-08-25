/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Core/Concurrency.h"
#include "Core/Memory.h"

#ifdef PLATFORM_WINDOWS

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <Windows.h>
#include <process.h>

NAMESPACE_CORE_BEGIN

//-----------------------------------//

#pragma region Threads

static bool ThreadIsValid(Thread* thread)
{
	return thread && thread->Handle;
}

bool ThreadJoin(Thread* thread)
{
	if( !ThreadIsValid(thread) || !thread->IsRunning )
		return false;
	
	thread->IsRunning = false;
	assert(thread->Handle);

	return ::WaitForSingleObject(thread->Handle, INFINITE) != WAIT_FAILED;
}

//-----------------------------------//

bool ThreadPause(Thread* thread)
{
	if( !ThreadIsValid(thread) )
		return false;

	return ::SuspendThread((HANDLE) thread->Handle) != -1;
}

//-----------------------------------//

bool ThreadResume(Thread* thread)
{
	if( !ThreadIsValid(thread) )
		return false;

	return ::ResumeThread((HANDLE) thread->Handle) != -1;
}

//-----------------------------------//

bool ThreadSetPriority(Thread* thread, ThreadPriority threadPriority)
{
	thread->Priority = threadPriority;

	int priority = 0;

	switch(threadPriority)
	{
	case ThreadPriority::Low:
		priority = THREAD_PRIORITY_BELOW_NORMAL;
		break;
	case ThreadPriority::Normal:
		priority = THREAD_PRIORITY_NORMAL;
		break;
	case ThreadPriority::High:
		priority = THREAD_PRIORITY_ABOVE_NORMAL;
		break;
	};

	return ::SetThreadPriority((HANDLE) thread->Handle, priority) != 0;
}

//-----------------------------------//

unsigned int WINAPI _ThreadMain(void* ptr)
{
	Thread* thread = (Thread*) ptr;
	thread->Function(thread, thread->Userdata);

	// _endthread automatically closes the thread handle.
	// ::CloseHandle((HANDLE) thread->Handle);
	
	thread->Handle = nullptr;
	thread->IsRunning = false;

	::_endthreadex(0);

	// Should not be reached.
	return 0;
}

//-----------------------------------//

bool ThreadStart(Thread* thread, ThreadFunction function, void* data)
{
	if (!thread || !function || thread->IsRunning)
		return false;

	thread->Function = function;
	thread->Userdata = data;

	// Create the thread suspended.
	uintptr_t handle = ::_beginthreadex(nullptr,
		0, _ThreadMain, thread, CREATE_SUSPENDED, nullptr);

	thread->Handle = (void*) handle;

	// State is set up, resume the thread.
	if( thread->Handle > 0 )
	{
		thread->IsRunning = true;
		ThreadSetPriority(thread, ThreadPriority::Normal);
		ThreadResume(thread);
	}

	return thread->Handle != 0;
}

//-----------------------------------//

static void SetThreadName( DWORD dwThreadID, LPCSTR szThreadName )
{
	struct THREADNAME_INFO
	{
		DWORD dwType;
		LPCSTR szName;
		DWORD dwThreadID;
		DWORD dwFlags;
	};

	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = szThreadName;
	info.dwThreadID = dwThreadID;
	info.dwFlags = 0;

	__try 
	{
		RaiseException( 0x406D1388, 0, sizeof(info)/sizeof(DWORD), (DWORD*)&info );
	}
	__except(EXCEPTION_CONTINUE_EXECUTION) { }
}

//-----------------------------------//

typedef DWORD (WINAPI *GetThreadIdFn)(HANDLE);

void ThreadSetName( Thread* thread, const String& name )
{
	if( !thread ) return;

	HMODULE module = GetModuleHandleA("Kernel32.dll");
	if( module == NULL ) return;

	// GetThreadId only exists on Vista or later versions, test if it exists
	// at runtime or the program will not run due to dynamic linking errors.

	GetThreadIdFn pGetThreadId = (GetThreadIdFn) GetProcAddress(module, "GetThreadId");
	if( pGetThreadId == NULL ) return;

	SetThreadName( pGetThreadId(thread->Handle), name.c_str() ); 
}

#pragma endregion

//-----------------------------------//

#pragma region Mutex

struct Mutex
{
	CRITICAL_SECTION Handle;
};

#define CS_CREATE_IMMEDIATELY_ON_WIN2000 0x080000000
#define CS_SPIN_COUNT 1500

//-----------------------------------//

Mutex* MutexCreate(Allocator* alloc)
{
	Mutex* mutex = Allocate(alloc, Mutex);
	MutexInit(mutex);

	return mutex;
}

//-----------------------------------//

API_CORE void MutexInit(Mutex* mutex)
{
	if (!mutex) return;

	LPCRITICAL_SECTION cs = (LPCRITICAL_SECTION) &mutex->Handle;
	
	BOOL result = ::InitializeCriticalSectionAndSpinCount(
		cs, CS_SPIN_COUNT | CS_CREATE_IMMEDIATELY_ON_WIN2000);
	
	assert(result && "Could not initialize critical section");
}

//-----------------------------------//

void MutexDestroy(Mutex* mutex)
{
	if (!mutex) return;

	LPCRITICAL_SECTION cs = (LPCRITICAL_SECTION) &mutex->Handle;
	DeleteCriticalSection(cs);

	Deallocate(mutex);
}

//-----------------------------------//

void MutexLock(Mutex* mutex)
{
	LPCRITICAL_SECTION cs = (LPCRITICAL_SECTION) &mutex->Handle;
	::EnterCriticalSection(cs);
}

//-----------------------------------//

void MutexUnlock(Mutex* mutex)
{
	LPCRITICAL_SECTION cs = (LPCRITICAL_SECTION) &mutex->Handle;
	::LeaveCriticalSection(cs);
}

#pragma endregion

//-----------------------------------//

#pragma region Condition Variables

/**
 * Windows only provides condition variables since Windows Vista,
 * so we must query those functions explicitly to be able to function
 * under Windows XP, though right now we provide no emulation.
 */

typedef VOID (WINAPI *InitializeConditionVariableFn)(PCONDITION_VARIABLE);
typedef BOOL (WINAPI *SleepConditionVariableFn)(PCONDITION_VARIABLE,
												PCRITICAL_SECTION, DWORD);
typedef VOID (WINAPI *WakeConditionVariableFn)(PCONDITION_VARIABLE);
typedef VOID (WINAPI *WakeAllConditionVariableFn)(PCONDITION_VARIABLE);

struct ConditionFunctions
{
	InitializeConditionVariableFn Init;
	SleepConditionVariableFn Sleep;
	WakeConditionVariableFn Wake;
	WakeAllConditionVariableFn WakeAll;
};

static ConditionFunctions* IntializeConditionFunctions()
{
	HMODULE module = GetModuleHandleA("Kernel32.dll");
	
	if( module == NULL )
		return 0;

	static ConditionFunctions functions;
	functions.Init = (InitializeConditionVariableFn)
		GetProcAddress(module, "InitializeConditionVariable");

	functions.Sleep = (SleepConditionVariableFn)
		GetProcAddress(module, "SleepConditionVariableCS");

	functions.Wake = (WakeConditionVariableFn)
		GetProcAddress(module, "WakeConditionVariable");
	
	functions.WakeAll = (WakeAllConditionVariableFn)
		GetProcAddress(module, "WakeAllConditionVariable");

	return &functions;
}

static ConditionFunctions* g_ConditionFunctions = IntializeConditionFunctions();

struct Condition
{
	CONDITION_VARIABLE Handle;
};

//-----------------------------------//

Condition* ConditionCreate(Allocator* alloc)
{
	Condition* cond = Allocate(alloc, Condition);
	ConditionInit(cond);

	return cond;
}

//-----------------------------------//

void ConditionDestroy(Condition* cond)
{
	Deallocate(cond);
}

//-----------------------------------//

void ConditionInit(Condition* cond)
{
	if( !cond ) return;

	if( !g_ConditionFunctions )
		return;

	CONDITION_VARIABLE* cvar = (CONDITION_VARIABLE*) &cond->Handle;
	
	if(g_ConditionFunctions->Init)
		g_ConditionFunctions->Init(cvar);
}

//-----------------------------------//

void ConditionWait(Condition* cond, Mutex* mutex)
{
	if( !cond || !mutex ) return;
	CONDITION_VARIABLE* cvar = (CONDITION_VARIABLE*) &cond->Handle;
	LPCRITICAL_SECTION cs = (LPCRITICAL_SECTION) &mutex->Handle;

	BOOL ret = FALSE;
	
	if(g_ConditionFunctions->Sleep) 
	{
		ret = g_ConditionFunctions->Sleep(cvar, cs, INFINITE);
		assert( ret != FALSE );
	}
}

//-----------------------------------//

void ConditionWakeOne(Condition* cond)
{
	if( !cond ) return;
	CONDITION_VARIABLE* cvar = (CONDITION_VARIABLE*) &cond->Handle;
	
	if(g_ConditionFunctions->Wake)
		g_ConditionFunctions->Wake(cvar);
}

//-----------------------------------//

void ConditionWakeAll(Condition* cond)
{
	if( !cond ) return;
	CONDITION_VARIABLE* cvar = (CONDITION_VARIABLE*) &cond->Handle;
	
	if(g_ConditionFunctions->WakeAll)
		g_ConditionFunctions->WakeAll(cvar);
}

#pragma endregion

//-----------------------------------//

#pragma region Atomics

int32 AtomicRead(volatile Atomic* atomic)
{
	return ::InterlockedExchangeAdd(atomic, 0);
}

int32 AtomicWrite(volatile Atomic* atomic, int32 value)
{
	return ::InterlockedExchange(atomic, value);
}

int32 AtomicAdd(volatile Atomic* atomic, int32 value)
{
	return ::InterlockedExchangeAdd(atomic, value);
}

int32 AtomicIncrement(volatile Atomic* atomic)
{
	return ::InterlockedIncrement(atomic);
}

int32 AtomicDecrement(volatile Atomic* atomic)
{
	return ::InterlockedDecrement(atomic);
}

#pragma endregion

//-----------------------------------//

NAMESPACE_CORE_END

#endif
