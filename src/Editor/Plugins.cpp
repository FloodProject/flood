/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "Plugin.h"
#include "PluginManager.h"

// Editor plugins
#include "plugins/Project/ProjectPlugin.h"
#include "plugins/UndoRedo/UndoPlugin.h"
#include "plugins/Scene/ScenePlugin.h"
#include "plugins/Log/LogPlugin.h"
#include "plugins/Console/ConsolePlugin.h"
#include "plugins/Resources/ResourcesPlugin.h"
#include "plugins/Property/PropertyPlugin.h"
#include "plugins/Selection/SelectionPlugin.h"
#include "plugins/Gizmos/GizmoPlugin.h"
#include "plugins/Terrain/TerrainPlugin.h"
#include "plugins/Camera/CameraPlugin.h"
#include "plugins/Mono/MonoPlugin.h"
#include "plugins/Sample/SamplePlugin.h"

namespace vapor { namespace editor {

//-----------------------------------//

#define PLUGIN(name) ReflectionGetType(name##Plugin);

void PluginManager::referencePlugins()
{
//	PLUGIN(Project);
//	PLUGIN(Undo);
//	PLUGIN(Scene);
//
//#ifdef ENABLE_PLUGIN_PROPERTY
//	PLUGIN(Property);
//#endif
//
//	PLUGIN(Selection);
//	//PLUGIN(Gizmo);
//	//PLUGIN(Terrain);
//	//PLUGIN(Camera);
//	PLUGIN(Console);
//	PLUGIN(Log);
//	PLUGIN(Resources);
//
//#ifdef ENABLE_PLUGIN_MONO
//	PLUGIN(Mono);
//#endif
//
//	PLUGIN(Sample);
}

//-----------------------------------//

} } // end namespaces