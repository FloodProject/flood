/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
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

//TODO this function might not be neccessary
static bool ThreadIsValid(Thread* thread)
{
	return thread && thread->handle;
}

bool Thread::join()
{
	if (!handle || !isRunning)
		return false;
	
	isRunning = false;
	assert(handle);

	return ::WaitForSingleObject(handle, INFINITE) != WAIT_FAILED;
}

//-----------------------------------//

bool Thread::pause()
{
	if (!handle)
		return false;

	return ::SuspendThread((HANDLE) handle) != -1;
}

//-----------------------------------//

bool Thread::resume()
{
	if (!handle)
		return false;

	return ::ResumeThread((HANDLE) handle) != -1;
}

//-----------------------------------//

bool Thread::setPriority(ThreadPriority priority)
{
	this->priority = priority;

	int pri = 0;

	switch(priority)
	{
	case ThreadPriority::Low:
		pri = THREAD_PRIORITY_BELOW_NORMAL;
		break;
	case ThreadPriority::Normal:
		pri = THREAD_PRIORITY_NORMAL;
		break;
	case ThreadPriority::High:
		pri = THREAD_PRIORITY_ABOVE_NORMAL;
		break;
	};

	return ::SetThreadPriority((HANDLE) handle, pri) != 0;
}

//-----------------------------------//

unsigned int WINAPI _ThreadMain(void* ptr)
{
	Thread* thread = (Thread*) ptr;
	thread->function(thread, thread->userdata);

	// _endthread automatically closes the thread handle.
	// ::CloseHandle((HANDLE) thread->Handle);
	
	thread->handle = nullptr;
	thread->isRunning = false;

	::_endthreadex(0);

	// Should not be reached.
	return 0;
}

//-----------------------------------//

bool Thread::start(ThreadFunction function, void* data)
{
	if (!function || isRunning)
		return false;

	this->function = function;
	userdata = data;

	// Create the thread suspended.
	uintptr_t handle = ::_beginthreadex(nullptr,
		0, _ThreadMain, this, CREATE_SUSPENDED, nullptr);

	this->handle = (void*) handle;

	// State is set up, resume the thread.
	if( this->handle > 0 )
	{
		isRunning = true;
		setPriority(ThreadPriority::Normal);
		resume();
	}

	return this->handle != 0;
}

//-----------------------------------//

static void SetThreadName(DWORD dwThreadID, LPCSTR szThreadName)
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
        RaiseException( 0x406D1388, 0, sizeof(info)/sizeof(DWORD), (const ULONG_PTR*)&info );
	}
	__except(EXCEPTION_CONTINUE_EXECUTION) { }
}

//-----------------------------------//

typedef DWORD (WINAPI *GetThreadIdFn)(HANDLE);

void Thread::setName(const char* name)
{
	HMODULE module = GetModuleHandleA("Kernel32.dll");
	if (module == NULL) return;

	// GetThreadId only exists on Vista or later versions, test if it exists
	// at runtime or the program will not run due to dynamic linking errors.

	GetThreadIdFn pGetThreadId = (GetThreadIdFn) GetProcAddress(module, "GetThreadId");
	if( pGetThreadId == NULL ) return;

	SetThreadName( pGetThreadId(handle), name ); 
}

#pragma endregion

//-----------------------------------//

#pragma region Mutex



#define CS_CREATE_IMMEDIATELY_ON_WIN2000 0x080000000
#define CS_SPIN_COUNT 1500

//-----------------------------------//

Mutex::Mutex()
{
	init();
}

//-----------------------------------//

Mutex::~Mutex()
{
	LPCRITICAL_SECTION cs = (LPCRITICAL_SECTION) handle;
	DeleteCriticalSection(cs);
	Deallocate(handle);
}

//-----------------------------------//

void Mutex::init()
{
	handle = AllocateHeap(CRITICAL_SECTION);
	LPCRITICAL_SECTION cs = (LPCRITICAL_SECTION) handle;
	
	BOOL result = ::InitializeCriticalSectionAndSpinCount(
		cs, CS_SPIN_COUNT | CS_CREATE_IMMEDIATELY_ON_WIN2000);
	
	assert(result && "Could not initialize critical section");
}

//-----------------------------------//

void Mutex::lock()
{
	LPCRITICAL_SECTION cs = (LPCRITICAL_SECTION) handle;
	::EnterCriticalSection(cs);
}

//-----------------------------------//

void Mutex::unlock()
{
	LPCRITICAL_SECTION cs = (LPCRITICAL_SECTION) handle;
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

//-----------------------------------//

Condition::Condition()
{
	init();
}

//-----------------------------------//

Condition::~Condition()
{
	Deallocate(handle);
}

//-----------------------------------//

void Condition::init()
{
	if (!g_ConditionFunctions)
		return;

	handle = AllocateHeap(CONDITION_VARIABLE);
	
	if (g_ConditionFunctions->Init)
		g_ConditionFunctions->Init(handle);
}

//-----------------------------------//

void Condition::wait(Mutex& mutex)
{

	LPCRITICAL_SECTION cs = (LPCRITICAL_SECTION) mutex.handle;
	BOOL ret = FALSE;
	
	if(g_ConditionFunctions->Sleep)
	{
		ret = g_ConditionFunctions->Sleep(handle, cs, INFINITE);
		assert( ret != FALSE );
	}
}

//-----------------------------------//

void Condition::wakeOne()
{
	if(g_ConditionFunctions->Wake)
		g_ConditionFunctions->Wake(handle);
}

//-----------------------------------//

void Condition::wakeAll()
{
	if(g_ConditionFunctions->WakeAll)
		g_ConditionFunctions->WakeAll(handle);
}

#pragma endregion

//-----------------------------------//

NAMESPACE_CORE_END

#endif
