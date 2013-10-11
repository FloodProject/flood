/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Memory.h"
#include "Core/Serialization.h"
#include "Core/SerializationHelpers.h"
#include "Core/Reflection.h"
#include "Core/Object.h"
#include "Core/Stream.h"
#include "ReflectionTypes.h"

#include <UnitTest++.h>

namespace
{

	void SerializeH(ReflectionContext* context, ReflectionWalkType wt)
	{
		H* h = (H*) context->object;

		if(context->loading)
		{
			context->primitive = &PrimitiveBuiltins::GetBuiltins().p_uint32;
			context->walkPrimitive(context, wt);
			h->hook = context->valueContext.u32;
		}
		else
		{
			context->walkComposite(context, ReflectionWalkType::Begin);

			// Serialize a custom integer value.
			context->valueContext.u32 = h->hook;
			context->primitive = &PrimitiveBuiltins::GetBuiltins().p_uint32;;
			context->walkPrimitive(context, wt);

			context->walkComposite(context, ReflectionWalkType::End);
		}
	}

	void SerializeHookI(ReflectionContext* context, ReflectionWalkType wt)
	{
		I* i = (I*) context->object;
		context->primitive = &PrimitiveBuiltins::GetBuiltins().p_uint32;;

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
	void ExecuteSerializationTest(SerializerType type, const char * ext)
	{
		Allocator* alloc = AllocatorGetHeap();

		ReflectionHandleContextMap handleContextMap;
		Serializer* serializer;
		
		switch(type)
		{
		case SerializerType::Json:
			serializer = Allocate(alloc, SerializerJSON, alloc, &handleContextMap);
			break;
		case SerializerType::Binary:
			serializer = Allocate(alloc, SerializerBinary, alloc, &handleContextMap);
			break;
		};

		B instanceB;
		instanceB.change();

		Serializer::saveObjectToFile(*serializer, StringFormat("TestB.%s", ext), &instanceB);

		serializer->object = nullptr;

		auto loadB = (B*) Serializer::loadObjectFromFile(*serializer, StringFormat("TestB.%s", ext));

		CHECK_EQUAL(instanceB.george, loadB->george);
		CHECK_EQUAL(instanceB.bar, loadB->bar);
		CHECK_CLOSE(instanceB.vec.z, loadB->vec.z, 0.01);
		CHECK_EQUAL(instanceB.str, loadB->str);

		Deallocate(loadB);

		C instanceC;
		instanceC.allocate();
		instanceC.change();

		Serializer::saveObjectToFile(*serializer, StringFormat("TestC.%s", ext), &instanceC);

		serializer->object = nullptr;

		C* loadC = (C*) Serializer::loadObjectFromFile(*serializer, StringFormat("TestC.%s", ext));

		CHECK_EQUAL(instanceC.anA->foo, loadC->anA->foo);
		CHECK_EQUAL(instanceC.arrayA.size(), loadC->arrayA.size());

		for(size_t i = 0; i < instanceC.arrayA.size(); i++)
			CHECK_EQUAL(instanceC.arrayA[i]->foo, loadC->arrayA[i]->foo);

		Deallocate(loadC);

		D instanceD;
		instanceD.allocate();
		Serializer::saveObjectToFile(*serializer, StringFormat("TestD.%s", ext), &instanceD);

		serializer->object = nullptr;

		D* loadD = (D*) Serializer::loadObjectFromFile(*serializer, StringFormat("TestD.%s", ext));
		Deallocate(loadD);

		F instanceF;
		instanceF.allocate();
		instanceF.a.foo = 42;
		Serializer::saveObjectToFile(*serializer, StringFormat("TestF.%s", ext), &instanceF);

		serializer->object = nullptr;

		F* loadF = (F*) Serializer::loadObjectFromFile(*serializer, StringFormat("TestF.%s", ext));
		CHECK_EQUAL(instanceF.vecA[0].foo, loadF->vecA[0].foo);
		CHECK_EQUAL(instanceF.vecA[1].foo, loadF->vecA[1].foo);
		CHECK_EQUAL(instanceF.a.foo, loadF->a.foo);
		Deallocate(loadF);

		Deallocate(serializer);
	}

	TEST(SerializeJSON)
	{
		ExecuteSerializationTest(SerializerType::Json, "json");
	}

	TEST(SerializeBinary)
	{
		ExecuteSerializationTest(SerializerType::Binary, "bin");
	}

	TEST(SerializeBinaryField)
	{
		Allocator* alloc = AllocatorGetHeap();
		MemoryStream ms(16);

		ReflectionHandleContextMap handleContextMap;
		auto bin = (SerializerBinary*) Allocate(alloc, SerializerBinary, alloc, &handleContextMap);
		bin->ms = &ms;

		A a;
		a.foo = 33;
		Class* classA = Class::GetType(&a);

		ReflectionContext& context = bin->serializeContext;
		context.object = &a;
		context.field = classA->getField("foo");
		CHECK(nullptr != context.field);

		ReflectionWalkCompositeField(&context);
		EncodeVariableInteger(bin->ms, Field::FieldInvalid);

		a.foo = 22;

		ReflectionContext& dcontext = bin->deserializeContext;
		dcontext.object = &a;
		dcontext.composite = classA;

		ms.setPosition( 0, StreamSeekMode::Absolute);
		DeserializeFields(&dcontext);
		CHECK_EQUAL(33, a.foo);

		Deallocate(bin);
	}

	TEST(SerializeBinaryHook)
	{
		Allocator* alloc = AllocatorGetHeap();

		ReflectionHandleContextMap handleContextMap;
		Serializer* serializer = Allocate(alloc, SerializerBinary, alloc, &handleContextMap);

		// Test custom type hook functions.
		H instanceH;
		instanceH.setup();
		Serializer::saveObjectToFile(*serializer, "TestH.bin", &instanceH);

		serializer->object = nullptr;

		H* loadH = (H*) Serializer::loadObjectFromFile(*serializer, "TestH.bin");
		CHECK_EQUAL(instanceH.hook, loadH->hook);
		Deallocate(loadH);

		// Test custom field hook functions.
		I instanceI;
		instanceI.setup();
		Serializer::saveObjectToFile(*serializer, "TestI.bin", &instanceI);

		serializer->object = nullptr;

		I* loadI = (I*) Serializer::loadObjectFromFile(*serializer, "TestI.bin");
		CHECK_EQUAL(instanceI.h.hook, loadI->h.hook);
		CHECK_EQUAL(instanceI.hook + 40, loadI->hook);
		Deallocate(loadI);

		Deallocate(serializer);
	}

	
}