/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Core/Plugin.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

REFLECT_ABSTRACT_CHILD_CLASS(Plugin, Object)
REFLECT_CLASS_END()

//-----------------------------------//

PluginMetadata::PluginMetadata()
	: startEnabled(true)
	, priority(1000)
{
}

//-----------------------------------//

Plugin::Plugin()
	: enabled(false)
{
}

//-----------------------------------//

Plugin::~Plugin()
{
}

//-----------------------------------//

void Plugin::doPluginDisable()
{
	onPluginDisable();
}

//-----------------------------------//

bool Plugin::isEnabled() const
{
	return enabled;
}

//-----------------------------------//

static bool SortPluginsCallback(Plugin* a, Plugin* b)
{
	int priorityA = a->getMetadata().priority;
	int priorityB = b->getMetadata().priority;

	return priorityA < priorityB;
}

void Plugin::sortByPriority(std::vector<Plugin*>& plugins)
{
	// Sort the plugins by priority.
	std::sort(plugins.begin(), plugins.end(), SortPluginsCallback);
}

//-----------------------------------//

NAMESPACE_CORE_END