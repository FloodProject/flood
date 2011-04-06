/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include <cstdio>
#include "CuTest.h"
#include "Core/Memory.h"

#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN
	#define NOMINMAX
	#include <Windows.h>
#endif

#define array_sizeof(n) (sizeof(n) / sizeof(n[0]))

using namespace vapor;

//-----------------------------------//

typedef CuSuite* (*SuiteFunction)();

CuSuite* GetSuitePath();
CuSuite* GetSuiteConcurrency();
CuSuite* GetSuiteStreams();
CuSuite* GetSuiteArchives();

static const SuiteFunction suites[] =
{
	GetSuitePath,
	GetSuiteConcurrency,
	GetSuiteStreams,
	GetSuiteArchives
};

//-----------------------------------//

void RunAllTests(void)
{
	CuSuite* suite = CuSuiteNew();
	
	for(int i = 0; i < array_sizeof(suites); i++)
	{
		SuiteFunction fn = suites[i];
		CuSuiteAddSuite(suite, fn());
	}
	
	CuSuiteRun(suite);

	CuString* output = CuStringNew();
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);

	printf("%s\n", output->buffer);

#ifdef WIN32
	OutputDebugStringA(output->buffer);
#endif

	CuSuiteDelete(suite);
	CuStringDelete(output);

	CuSuiteFreeAll();
}

//-----------------------------------//

int main(void)
{
	CuInit();
	RunAllTests();
	CuCleanup();

	AllocatorDumpInfo();

	return 0;
}
