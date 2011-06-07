/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "CuTest.h"

#include "Core/API.h"
#include "Core/Memory.h"
#include "Core/Serialization.h"
#include "Core/Reflection.h"
#include "Core/Object.h"
#include "Core/Stream.h"
#include "ReflectionTypes.h"

using namespace vapor;

//-----------------------------------//

static Object* LoadObjectFromFile(const Path& file)
{
	Serializer* json = SerializerCreateJSON( AllocatorGetHeap() );
	json->alloc = AllocatorGetHeap();

	Stream* stream = StreamCreateFromFile(AllocatorGetHeap(), file.c_str(), StreamMode::Read);	
	json->stream = stream;

	Object* object = SerializerLoad(json);

	SerializerDestroy(json);
	StreamDestroy(stream);

	return object;
}

//-----------------------------------//

static void SaveObjectToFile(const Path& file, Object* object)
{
	Serializer* json = SerializerCreateJSON( AllocatorGetHeap() );
	json->alloc = AllocatorGetHeap();

	Stream* stream = StreamCreateFromFile(AllocatorGetHeap(), file.c_str(), StreamMode::Write);	
	json->stream = stream;
	json->object = object;
	
	SerializerSave(json);

	SerializerDestroy(json);
	StreamDestroy(stream);
}

//-----------------------------------//

void TestSerializerJSON(CuTest* tc)
{
#if 0
	B instanceB;
	SaveObjectToFile("TestB.json", &instanceB);

	B* loadB = (B*) LoadObjectFromFile("TestB.json");
	Deallocate(loadB);
#endif

	C instanceC;
	instanceC.change();
	SaveObjectToFile("TestC.json", &instanceC);

	C* loadC = (C*) LoadObjectFromFile("TestC.json");
	Deallocate(loadC);
}

//-----------------------------------//

CuSuite* GetSuiteSerialization()
{
    CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, TestSerializerJSON);
    return suite;	
}