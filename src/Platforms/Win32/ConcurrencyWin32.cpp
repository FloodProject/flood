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

bool ThreadJoin(Thread* thread)
{
	if( !thread || !thread->IsRunning )
		return false;
	
	thread->IsRunning = false;
	assert(thread->Handle);

	return ::WaitForSingleObject(thread->Handle, INFINITE) != WAIT_FAILED;
}

//-----------------------------------//

bool ThreadPause(Thread* thread)
{
	if( !thread ) return false;
	return ::SuspendThread((HANDLE) thread->Handle) != -1;
}

//-----------------------------------//

bool ThreadResume(Thread* thread)
{
	if( !thread ) return false;
	return ::ResumeThread((HANDLE) thread->Handle) != -1;
}

//-----------------------------------//

bool ThreadSetPriority(Thread* thread, ThreadPriority priority)
{
	thread->Priority = priority;

	// Normal priority is 0 under Windows.
	return ::SetThreadPriority((HANDLE) thread->Handle, thread->Priority) != 0;
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
	uintptr_t hnd = ::_beginthreadex(nullptr, 0, _ThreadMain, thread, CREATE_SUSPENDED, nullptr);
	thread->Handle = (void*) hnd;

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

//-----------------------------------//

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
	LPCRITICAL_SECTION cs = (LPCRITICAL_SECTION) &mutex->Handle;
	::InitializeCriticalSectionAndSpinCount(cs, CS_SPIN_COUNT | CS_CREATE_IMMEDIATELY_ON_WIN2000);
}

//-----------------------------------//

void MutexDestroy(Mutex* mutex)
{
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

//-----------------------------------//

typedef VOID (WINAPI *InitializeConditionVariableFn)(PCONDITION_VARIABLE);
typedef BOOL (WINAPI *SleepConditionVariableCSFn)(PCONDITION_VARIABLE, PCRITICAL_SECTION, DWORD);
typedef VOID (WINAPI *WakeConditionVariableFn)(PCONDITION_VARIABLE);
typedef VOID (WINAPI *WakeAllConditionVariableFn)(PCONDITION_VARIABLE);

static InitializeConditionVariableFn pInitializeConditionVariable = nullptr;
static SleepConditionVariableCSFn pSleepConditionVariableCS = nullptr;
static WakeConditionVariableFn pWakeConditionVariable = nullptr;
static WakeAllConditionVariableFn pWakeAllConditionVariable = nullptr;

static bool IntializeConditionVars()
{
	HMODULE module = GetModuleHandleA("Kernel32.dll");
	if( module == NULL ) return true;

	pInitializeConditionVariable = (InitializeConditionVariableFn) GetProcAddress(module, "InitializeConditionVariable");
	pSleepConditionVariableCS = (SleepConditionVariableCSFn) GetProcAddress(module, "SleepConditionVariableCS");
	pWakeConditionVariable = (WakeConditionVariableFn) GetProcAddress(module, "WakeConditionVariable");
	pWakeAllConditionVariable = (WakeAllConditionVariableFn) GetProcAddress(module, "WakeAllConditionVariable");

	return true;
}

static bool g_InitCondVars = false;

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

	// Lazy initialization of the condition variable functions.
	if( !g_InitCondVars )
		g_InitCondVars = IntializeConditionVars();

	CONDITION_VARIABLE* cvar = (CONDITION_VARIABLE*) &cond->Handle;
	if(pInitializeConditionVariable) pInitializeConditionVariable(cvar);
}

//-----------------------------------//

void ConditionWait(Condition* cond, Mutex* mutex)
{
	if( !cond || !mutex ) return;
	CONDITION_VARIABLE* cvar = (CONDITION_VARIABLE*) &cond->Handle;
	LPCRITICAL_SECTION cs = (LPCRITICAL_SECTION) &mutex->Handle;

	BOOL ret = FALSE;
	
	if(pSleepConditionVariableCS) 
	{
		ret = pSleepConditionVariableCS(cvar, cs, INFINITE);
		assert( ret != FALSE );
	}
}

//-----------------------------------//

void ConditionWakeOne(Condition* cond)
{
	if( !cond ) return;
	CONDITION_VARIABLE* cvar = (CONDITION_VARIABLE*) &cond->Handle;
	if(pWakeConditionVariable) pWakeConditionVariable(cvar);
}

//-----------------------------------//

void ConditionWakeAll(Condition* cond)
{
	if( !cond ) return;
	CONDITION_VARIABLE* cvar = (CONDITION_VARIABLE*) &cond->Handle;
	if(pWakeAllConditionVariable) pWakeAllConditionVariable(cvar);
}

//-----------------------------------//

int32 AtomicRead(volatile Atomic* atomic) { return ::InterlockedExchangeAdd(atomic, 0); }
int32 AtomicWrite(volatile Atomic* atomic, int32 value) { return ::InterlockedExchange(atomic, value); }
int32 AtomicAdd(volatile Atomic* atomic, int32 value) { return ::InterlockedExchangeAdd(atomic, value); }
int32 AtomicIncrement(volatile Atomic* atomic) { return ::InterlockedIncrement(atomic); }
int32 AtomicDecrement(volatile Atomic* atomic) { return ::InterlockedDecrement(atomic); }

//-----------------------------------//

NAMESPACE_CORE_END

#endif
