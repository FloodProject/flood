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
#include "Core/SerializationHelpers.h"
#include "Core/Reflection.h"
#include "Core/Object.h"
#include "Core/Stream.h"
#include "ReflectionTypes.h"

//-----------------------------------//

typedef Serializer* (*SerializerCreateFunction)(Allocator*);

void TestSerialization(CuTest* tc, SerializerCreateFunction SerializerCreate, const char* ext)
{
	Allocator* alloc = AllocatorGetHeap();
	Serializer* serializer = SerializerCreate(alloc);

	B instanceB;
	instanceB.change();
	
	SerializerSaveObjectToFile(serializer, StringFormat("TestB.%s", ext), &instanceB);

	B* loadB = (B*) SerializerLoadObjectFromFile(serializer, StringFormat("TestB.%s", ext));

	CuAssertIntEquals(tc, instanceB.george, loadB->george);
	CuAssertIntEquals(tc, instanceB.bar, loadB->bar);
	CuAssertDblEquals(tc, instanceB.vec.z, loadB->vec.z, 0.01);
	CuAssertCppStrEquals(tc, instanceB.str, loadB->str);

	Deallocate(loadB);

	C instanceC;
	instanceC.allocate();
	instanceC.change();

	SerializerSaveObjectToFile(serializer, StringFormat("TestC.%s", ext), &instanceC);

	C* loadC = (C*) SerializerLoadObjectFromFile(serializer, StringFormat("TestC.%s", ext));

	CuAssertIntEquals(tc, instanceC.anA->foo, loadC->anA->foo);
	CuAssertIntEquals(tc, instanceC.arrayA.size(), loadC->arrayA.size());

	for(size_t i = 0; i < instanceC.arrayA.size(); i++)
		CuAssertIntEquals(tc, instanceC.arrayA[i]->foo, loadC->arrayA[i]->foo);

	Deallocate(loadC);

	D instanceD;
	instanceD.allocate();
	SerializerSaveObjectToFile(serializer, StringFormat("TestD.%s", ext), &instanceD);
	
	D* loadD = (D*) SerializerLoadObjectFromFile(serializer, StringFormat("TestD.%s", ext));
	Deallocate(loadD);

	F instanceF;
	instanceF.allocate();
	instanceF.a.foo = 42;
	SerializerSaveObjectToFile(serializer, StringFormat("TestF.%s", ext), &instanceF);
	
	F* loadF = (F*) SerializerLoadObjectFromFile(serializer, StringFormat("TestF.%s", ext));
	CuAssertIntEquals(tc, instanceF.vecA[0].foo, loadF->vecA[0].foo);
	CuAssertIntEquals(tc, instanceF.vecA[1].foo, loadF->vecA[1].foo);
	CuAssertIntEquals(tc, instanceF.a.foo, loadF->a.foo);
	Deallocate(loadF);

	Deallocate(serializer);
}

void TestSerializerJSON(CuTest* tc)
{
	TestSerialization(tc, SerializerCreateJSON, "json");
}

void TestSerializerBinary(CuTest* tc)
{
	TestSerialization(tc, SerializerCreateBinary, "bin");
}

//-----------------------------------//

void TestSerializerBinaryField(CuTest* tc)
{
	Allocator* alloc = AllocatorGetHeap();
	MemoryStream* ms = StreamCreateFromMemory(alloc, 16);

	SerializerBinary* bin = (SerializerBinary*) SerializerCreateBinary(alloc);
	bin->ms = ms;

	A a;
	a.foo = 33;
	Class* classA = ClassGetType(&a);
	
	ReflectionContext& context = bin->serializeContext;
	context.object = &a;
	context.field = ClassGetField(classA, "foo");
	CuAssertPtrNotNull(tc, context.field);

	ReflectionWalkCompositeField(&context);
	EncodeVariableInteger(bin->ms, FieldInvalid);

	a.foo = 22;

	ReflectionContext& dcontext = bin->deserializeContext;
	dcontext.object = &a;
	dcontext.composite = classA;

	StreamSetPosition(ms, 0, StreamSeekMode::Absolute);
	DeserializeFields(&dcontext);
	CuAssertIntEquals(tc, 33, a.foo);

	Deallocate(ms);
	Deallocate(bin);
}

//-----------------------------------//

void TestSerializerBinaryHook(CuTest* tc)
{
	Allocator* alloc = AllocatorGetHeap();
	Serializer* serializer = SerializerCreateBinary(alloc);

	// Test custom type hook functions.
	H instanceH;
	instanceH.setup();
	SerializerSaveObjectToFile(serializer, "TestH.bin", &instanceH);

	H* loadH = (H*) SerializerLoadObjectFromFile(serializer, "TestH.bin");
	CuAssertIntEquals(tc, instanceH.hook, loadH->hook);
	Deallocate(loadH);

	// Test custom field hook functions.
	I instanceI;
	instanceI.setup();
	SerializerSaveObjectToFile(serializer, "TestI.bin", &instanceI);

	I* loadI = (I*) SerializerLoadObjectFromFile(serializer, "TestI.bin");
	CuAssertIntEquals(tc, instanceI.h.hook, loadI->h.hook);
	CuAssertIntEquals(tc, instanceI.hook + 40, loadI->hook);
	Deallocate(loadI);

	Deallocate(serializer);
}

//-----------------------------------//

void SerializeH(ReflectionContext* context, ReflectionWalkType::Enum wt)
{
	H* h = (H*) context->object;

	if(context->loading)
	{
		context->primitive = &Primitive::s_uint32;
		context->walkPrimitive(context, wt);
		h->hook = context->valueContext.u32;
	}
	else
	{
		context->walkComposite(context, ReflectionWalkType::Begin);

		// Serialize a custom integer value.
		context->valueContext.u32 = h->hook;
		context->primitive = &Primitive::s_uint32;
		context->walkPrimitive(context, wt);

		context->walkComposite(context, ReflectionWalkType::End);
	}
}

REFLECT_CHILD_CLASS(H, Object)
	REFLECT_CLASS_SET_SERIALIZER(SerializeH)
REFLECT_CLASS_END()

void SerializeHookI(ReflectionContext* context, ReflectionWalkType::Enum wt)
{
	I* i = (I*) context->object;
	context->primitive = &Primitive::s_uint32;

	if(context->loading)
	{
		context->walkPrimitive(context, wt);
		i->hook = context->valueContext.u32 + 20;
	}
	else
	{
		// Serialize a custom integer value.
		context->valueContext.u32 = i->hook + 20;
		context->walkPrimitive(context, wt);
	}
}

REFLECT_CHILD_CLASS(I, Object)
	FIELD_CLASS(2, H, h)
	FIELD_PRIMITIVE(3, uint32, hook) FIELD_SET_SERIALIZER(hook, SerializeHookI)
REFLECT_CLASS_END()

//-----------------------------------//

CuSuite* GetSuiteSerialization()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, TestSerializerJSON);
	SUITE_ADD_TEST(suite, TestSerializerBinary);
	SUITE_ADD_TEST(suite, TestSerializerBinaryField);
	SUITE_ADD_TEST(suite, TestSerializerBinaryHook);
	return suite;
}