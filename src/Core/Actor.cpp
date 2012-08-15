//////////////////////////////////////////////////////////////////////////
//
// Flood Engine © (2008-2013)
// <https://github.com/tritao/flood>
//
//////////////////////////////////////////////////////////////////////////

#include "Core/API.h"
#include "Core/Actor.h"

#include <algorithm>

namespace _detail {

	std::vector<Actor *> g_Actors;

}

NAMESPACE_CORE_BEGIN

	Actor * ActorSpawn()
	{
		Actor * actor = AllocateHeap(Actor);
		_detail::g_Actors.push_back(actor);

		return actor;
	}

	void ActorDespawn(Actor * actor)
	{
		auto iter = std::find(_detail::g_Actors.begin(), _detail::g_Actors.end(), actor);
		
		if(iter != _detail::g_Actors.end())
			_detail::g_Actors.erase(iter);
	}

NAMESPACE_CORE_END