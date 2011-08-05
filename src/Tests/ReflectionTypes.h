/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/Reflection.h"
#include "Math/Vector.h"
#include "Math/Quaternion.h"
#include "Math/Color.h"

//-----------------------------------//

REFLECT_DECLARE_ENUM(E)

enum_class E
{
	F1 = 0, F2, F3, Max
};

//-----------------------------------//

REFLECT_DECLARE_CLASS(A)

struct A : public Object
{
	REFLECT_DECLARE_OBJECT(A)

	A() : foo(20), foos(E::F1) { }
	virtual ~A() { }

	int32 foo;
	E foos;
};

//-----------------------------------//

REFLECT_DECLARE_CLASS(B)

struct B : public A
{
	REFLECT_DECLARE_OBJECT(B)

	B()
		: george(false)
		, bar(5)
		, vec(1, 2, 3)
		, quat(5, 6, 7, 1)
		, color(1.1f, 1.2f, 1.3f, 1.4f)
		, str("serialized string")
	{ }

	void change()
	{
		george = true;
		bar += 2;
		vec.z = 33;
		str = "ole";
	}

	bool george;
	uint32 bar;
	Vector3 vec;
	Quaternion quat;
	Color color;
	String str;
};

//-----------------------------------//

REFLECT_DECLARE_CLASS(C)

#define NUM_AS 5

struct C : public Object
{
	REFLECT_DECLARE_OBJECT(C)

	C() : anA(0)
	{
	}

	~C()
	{
		Deallocate(anA);
		for(size_t i = 0; i < arrayA.size(); i++)
			Deallocate(arrayA[i]);
	}

	void allocate()
	{
		anA = Allocate(A, AllocatorGetThis());
		
		for(size_t i = 0; i < NUM_AS; i++)
		{
			A* a = Allocate(A, AllocatorGetThis());
			arrayA.push_back(a);
		}
	}

	void change()
	{
		anA->foo = 42;

		for(size_t i = 0; i < NUM_AS; i++)
		{
			A* a = arrayA[i];
			a->foo += i;
		}
	}

	A* anA;
	std::vector<A*> arrayA;
};

//-----------------------------------//
