/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

NAMESPACE_SERVER_BEGIN

//-----------------------------------//

#define RANDSIZL   (8)
#define RANDSIZ    (1<<RANDSIZL)

class CipherISAAC
{
public:

	CipherISAAC();

	// Initializes the cipher.
	void init(bool flag);

	// Seeds the cipher.
	void seed(uint32 seed[256]);

	// Generates a new random number from the cipher.
	void isaac();

public:

	uint32 randcnt;
	uint32 randrsl[RANDSIZ];
	uint32 randmem[RANDSIZ];
	uint32 randa;
	uint32 randb;
	uint32 randc;
};

//-----------------------------------//

NAMESPACE_SERVER_END