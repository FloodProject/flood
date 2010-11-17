/************************************************************************
*
* vapor3D Project © (2008-2010)
*
*	<http://www.vapor3d.org>
*
*/

#pragma once

namespace vapor {

//-----------------------------------//

/**
 * Performs atomic operations on 32-bit integral types.
 * Useful for safe and fast multithreaded reference counts.
 */

class Atomic
{
	DECLARE_UNCOPYABLE(Atomic)

public:

	Atomic(uint val);
	~Atomic();

	// Gets a value atomically.
	uint get() const;

	// Sets a value atomically.
	uint set(uint val);

	// Adds a value atomically.
	uint add(uint val);

	// Subtracts a value atomically.
	uint sub(uint val);

	// Increments the value atomically.	
	uint inc();

	// Drecrements the value atomically.
	uint dec();

private:
	
	VAPOR_ALIGN_BEGIN(32)
		volatile uint value;
	VAPOR_ALIGN_END(32)
};

//-----------------------------------//

} // end namespace

