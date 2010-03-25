/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

namespace vapor {

//-----------------------------------//

class VAPOR_API Task : public ReferenceCounted,
	private boost::noncopyable
{
public:

	virtual ~Task() { }
	virtual void run() = 0;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Task );

//-----------------------------------//

} // end namespace
	