/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

namespace vapor {

//-----------------------------------//

/**
 * Template that provides singleton functionality for classes.
 * (based on OGRE's Singleton template code).
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
		assert( m_Instance );
		return ( *m_Instance );
	}
	
	static T* getInstancePtr()
	{
		return m_Instance;
	}

protected:

	static T* m_Instance;
};

template <typename T> T* Singleton<T>::m_Instance = nullptr;

//-----------------------------------//

} // end namespace