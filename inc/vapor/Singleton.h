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

template <typename T> class Singleton
{
public:

	Singleton()
	{
		assert( !m_Instance );
		m_Instance = static_cast< T* >( this );
	}

	~Singleton()
	{  
		assert( m_Instance );
		m_Instance = 0; 
	}
	
	static T& getInstance()
	{	
		if( !m_Instance ) new T();

		assert( m_Instance );
		return ( *m_Instance );
	}
	
	static T* getInstancePtr()
	{
		if( !m_Instance ) new T();

		assert( m_Instance );
		return m_Instance;
	}

protected:

	static T* m_Instance;
};

template <typename T> T* Singleton<T>::m_Instance = nullptr;

//-----------------------------------//

} // end namespace