/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Plugin.h"
#include "Core/Containers/Sort.h"
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

void PluginsSortByPriority(Vector<Plugin*>& plugins)
{
    // Sort the plugins by priority.
    Sort(plugins.begin(), plugins.end(), SortPluginsCallback);
}

//-----------------------------------//

NAMESPACE_CORE_END