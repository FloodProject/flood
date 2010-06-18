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

/**
 * Template that provides singleton functionality for classes.
 * (based on OGRE's Singleton and CodeProject's template code).
 */

template<typename T>
class Singleton
{
public:

	Singleton()
	{
		assert( !instance );
		instance = static_cast<T*>( this );
	}

	~Singleton()
	{  
		assert( instance );
		instance = nullptr; 
	}
	
	static T& getInstance()
	{	
		if( !instance )
			new T();

		assert( instance );
		return *instance;
	}
	
	static T* getInstancePtr()
	{
		if( !instance )
			new T();

		assert( instance );
		return instance;
	}

protected:

	static T* instance;
};

template<typename T>
T* Singleton<T>::instance = nullptr;

//-----------------------------------//

} // end namespace