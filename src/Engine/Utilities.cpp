/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"

namespace vapor {

//-----------------------------------//

// Check for mode details about endianness-related tricks and issues:
// http://www.gamedev.net/reference/articles/article2091.asp

	bool isLittleEndian()
{
	byte EndianTest[2] = { 1, 0 };
	short x;
	x = *(short *) EndianTest;

	return x == 1; // Little Endian
}

//-----------------------------------//

long endian_swap(long i)
{
	unsigned char b1, b2, b3, b4;

	b1 = i & 255;
	b2 = ( i>>8 )  & 255;
	b3 = ( i>>16 ) & 255;
	b4 = ( i>>24 ) & 255;

	return ((int)b1 << 24) + ((int)b2 << 16) + ((int)b3 << 8) + b4;
}

//-----------------------------------//

void float_to_str( char* str, float n, byte precision )
{
//#ifdef VAPOR_PLATFORM_WINDOWS
//	sprintf_s( sprintf( str, 32, "%#.*f", precision, n );
//#else
	sprintf( str, "%#.*f", precision, n );
//#endif
}

//-----------------------------------//

#ifdef VAPOR_PLATFORM_WINDOWS

std::string wstr_to_str(const std::wstring &wstr)
{
    // Convert a Unicode string to an ASCII string
    std::string strTo;
    char *szTo = new char[wstr.length() + 1];
    szTo[wstr.size()] = '\0';
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, szTo,
		(int)wstr.length(), nullptr, nullptr);
    strTo = szTo;
    delete[] szTo;
    return strTo;
}

#endif

//-----------------------------------//

#ifdef VAPOR_PLATFORM_WINDOWS

std::wstring str_to_wstr(const std::string &str)
{
    // Convert an ASCII string to a Unicode String
    std::wstring wstrTo;
    wchar_t *wszTo = new wchar_t[str.length() + 1];
    wszTo[str.size()] = L'\0';
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wszTo,
		(int)str.length());
    wstrTo = wszTo;
    delete[] wszTo;
    return wstrTo;
}

#endif

//-----------------------------------//

std::vector<std::string>& str_split(const std::string &s, char delim, 
								std::vector<std::string> &elems)
{
    std::stringstream ss(s);
    std::string item;
    
	while(std::getline(ss, item, delim)) 
	{
        elems.push_back(item);
    }
    
	return elems;
}

//-----------------------------------//

std::vector<std::string> str_split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    return str_split(s, delim, elems);
}

////-----------------------------------//
//
//#ifdef VAPOR_PLATFORM_WINDOWS
//
//#define MS_VC_EXCEPTION 0x406D1388
//
//VAPOR_ALIGN_BEGIN(8) struct tagTHREADNAME_INFO
//{
//	DWORD dwType;	// Must be 0x1000.
//	LPCSTR szName;	// Pointer to name (in user addr space).
//	DWORD dwThreadID;// Thread ID (-1 = caller thread).
//	DWORD dwFlags;	// Reserved for future use, must be zero.
//} VAPOR_ALIGN_END(8);
//
//typedef struct tagTHREADNAME_INFO THREADNAME_INFO;
//
//void SetThreadName( ulong dwThreadID, const std::string& threadName )
//{
//   Sleep(10);
//   THREADNAME_INFO info;
//   info.dwType = 0x1000;
//   info.szName = threadName.c_str();
//   info.dwThreadID = (DWORD) dwThreadID;
//   info.dwFlags = 0;
//
//   __try
//   {
//      RaiseException( MS_VC_EXCEPTION, 0, 
//		  sizeof(info)/sizeof(ULONG_PTR), (ULONG_PTR*)&info );
//   }
//   __except(EXCEPTION_EXECUTE_HANDLER)
//   {
//   }
//}
//
////-----------------------------------//
//
//typedef struct CLIENT_ID
//{
//	PVOID UniqueProcess;
//	PVOID UniqueThread;
//} CLIENT_ID;
//
//typedef LONG    NTSTATUS;
//typedef LONG    KPRIORITY;
//
//typedef struct _THREAD_BASIC_INFORMATION
//{
//	NTSTATUS	ExitStatus;
//	PVOID		TebBaseAddress;
//	CLIENT_ID	ClientId;
//	KAFFINITY	AffinityMask;
//	KPRIORITY	Priority;
//	KPRIORITY	BasePriority;
//} THREAD_BASIC_INFORMATION;
//
//typedef enum _THREAD_INFORMATION_CLASS 
//{
//    ThreadBasicInformation,
//    ThreadTimes,
//    ThreadPriority,
//    ThreadBasePriority,
//    ThreadAffinityMask,
//    ThreadImpersonationToken,
//    ThreadDescriptorTableEntry,
//    ThreadEnableAlignmentFaultFixup,
//    ThreadEventPair,
//    ThreadQuerySetWin32StartAddress,
//    ThreadZeroTlsCell,
//    ThreadPerformanceCount,
//    ThreadAmILastThread,
//    ThreadIdealProcessor,
//    ThreadPriorityBoost,
//    ThreadSetTlsArrayAddress,
//    ThreadIsIoPending,
//    ThreadHideFromDebugger
//} THREAD_INFORMATION_CLASS, *PTHREAD_INFORMATION_CLASS;
//
//NTSTATUS NTSYSAPI NTAPI NtQueryInformationThread(
//  IN HANDLE               ThreadHandle,
//  IN THREAD_INFORMATION_CLASS ThreadInformationClass,
//  OUT PVOID               ThreadInformation,
//  IN ULONG                ThreadInformationLength,
//  OUT PULONG              ReturnLength OPTIONAL );
//
//ulong GetThreadId(HANDLE Thread)
//{
//    THREAD_BASIC_INFORMATION tbi;
//    NTSTATUS status;
//
//    status = NtQueryInformationThread(Thread, ThreadBasicInformation, &tbi,
//                                      sizeof(tbi), nullptr);
//    if (status)
//		error( "thread::GetThreadId", "NtQueryInformationThread error" );
//
//    return HandleToULong(tbi.ClientId.UniqueThread);
//}
//
//#endif

//-----------------------------------//

} // end namespace