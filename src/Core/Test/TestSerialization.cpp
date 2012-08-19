//////////////////////////////////////////////////////////////////////////
//
// Flood © (2008-2013)
// CoreSerialization - Converted serialization tests from vapor3d
//
//////////////////////////////////////////////////////////////////////////

#include "Core/API.h"
#include "UnitTest++.h"

#include "Core/Memory.h"
#include "Core/Serialization.h"
#include "Core/SerializationHelpers.h"
#include "Core/Reflection.h"
#include "Core/Object.h"
#include "Core/Stream.h"
#include "ReflectionTypes.h"

namespace
{
	typedef Serializer* (*SerializerCreateFunction)(Allocator*, ReflectionHandleContextMap*);

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
}

REFLECT_CHILD_CLASS(H, Object)
	REFLECT_CLASS_SET_SERIALIZER(SerializeH)
REFLECT_CLASS_END()

REFLECT_CHILD_CLASS(I, Object)
	FIELD_CLASS(2, H, h)
	FIELD_PRIMITIVE(3, uint32, hook) FIELD_SET_SERIALIZER(hook, SerializeHookI)
REFLECT_CLASS_END()

SUITE(Core)
{
	void ExecuteSerializationTest(SerializerCreateFunction SerializerCreate, const char * ext)
	{
		Allocator* alloc = AllocatorGetHeap();

		ReflectionHandleContextMap handleContextMap;
		Serializer* serializer = SerializerCreate(alloc, &handleContextMap);

		B instanceB;
		instanceB.change();

		SerializerSaveObjectToFile(serializer, StringFormat("TestB.%s", ext), &instanceB);

		B* loadB = (B*) SerializerLoadObjectFromFile(serializer, StringFormat("TestB.%s", ext));

		CHECK_EQUAL(instanceB.george, loadB->george);
		CHECK_EQUAL(instanceB.bar, loadB->bar);
		CHECK_CLOSE(instanceB.vec.z, loadB->vec.z, 0.01);
		CHECK_EQUAL(instanceB.str, loadB->str);

		Deallocate(loadB);

		C instanceC;
		instanceC.allocate();
		instanceC.change();

		SerializerSaveObjectToFile(serializer, StringFormat("TestC.%s", ext), &instanceC);

		C* loadC = (C*) SerializerLoadObjectFromFile(serializer, StringFormat("TestC.%s", ext));

		CHECK_EQUAL(instanceC.anA->foo, loadC->anA->foo);
		CHECK_EQUAL(instanceC.arrayA.size(), loadC->arrayA.size());

		for(size_t i = 0; i < instanceC.arrayA.size(); i++)
			CHECK_EQUAL(instanceC.arrayA[i]->foo, loadC->arrayA[i]->foo);

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
		CHECK_EQUAL(instanceF.vecA[0].foo, loadF->vecA[0].foo);
		CHECK_EQUAL(instanceF.vecA[1].foo, loadF->vecA[1].foo);
		CHECK_EQUAL(instanceF.a.foo, loadF->a.foo);
		Deallocate(loadF);

		Deallocate(serializer);
	}

	TEST(SerializeJSON)
	{
		ExecuteSerializationTest(SerializerCreateJSON, "json");
	}

	TEST(SerializeBinary)
	{
		ExecuteSerializationTest(SerializerCreateBinary, "bin");
	}

	TEST(SerializeBinaryField)
	{
		Allocator* alloc = AllocatorGetHeap();
		MemoryStream* ms = StreamCreateFromMemory(alloc, 16);

		ReflectionHandleContextMap handleContextMap;
		auto bin = (SerializerBinary*) SerializerCreateBinary(alloc, &handleContextMap);
		bin->ms = ms;

		A a;
		a.foo = 33;
		Class* classA = ClassGetType(&a);

		ReflectionContext& context = bin->serializeContext;
		context.object = &a;
		context.field = ClassGetField(classA, "foo");
		CHECK(nullptr != context.field);

		ReflectionWalkCompositeField(&context);
		EncodeVariableInteger(bin->ms, FieldInvalid);

		a.foo = 22;

		ReflectionContext& dcontext = bin->deserializeContext;
		dcontext.object = &a;
		dcontext.composite = classA;

		StreamSetPosition(ms, 0, StreamSeekMode::Absolute);
		DeserializeFields(&dcontext);
		CHECK_EQUAL(33, a.foo);

		Deallocate(ms);
		Deallocate(bin);
	}

	TEST(SerializeBinaryHook)
	{
		Allocator* alloc = AllocatorGetHeap();

		ReflectionHandleContextMap handleContextMap;
		Serializer* serializer = SerializerCreateBinary(alloc, &handleContextMap);

		// Test custom type hook functions.
		H instanceH;
		instanceH.setup();
		SerializerSaveObjectToFile(serializer, "TestH.bin", &instanceH);

		H* loadH = (H*) SerializerLoadObjectFromFile(serializer, "TestH.bin");
		CHECK_EQUAL(instanceH.hook, loadH->hook);
		Deallocate(loadH);

		// Test custom field hook functions.
		I instanceI;
		instanceI.setup();
		SerializerSaveObjectToFile(serializer, "TestI.bin", &instanceI);

		I* loadI = (I*) SerializerLoadObjectFromFile(serializer, "TestI.bin");
		CHECK_EQUAL(instanceI.h.hook, loadI->h.hook);
		CHECK_EQUAL(instanceI.hook + 40, loadI->hook);
		Deallocate(loadI);

		Deallocate(serializer);
	}

	
}