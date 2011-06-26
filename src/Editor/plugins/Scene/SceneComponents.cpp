/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "ScenePane.h"
#include "Core/Utilities.h"
#include "Events.h"
#include "Editor.h"
#include "EditorIcons.h"
#include "Viewframe.h"
#include "UndoManager.h"
#include "Scene/Projector.h"
#include "Render/Cube.h"

namespace vapor { namespace editor {

//-----------------------------------//

struct ComponentEntry
{
	const char* category;
	const Type* type;
	const byte* icon;
	const int icon_length;
};

#define TYPE(T) ReflectionGetType(T)
#define BMP(T) T, sizeof(T)

static ComponentEntry components[] =
{
	{ nullptr, TYPE(Model),				BMP(shape_flip_horizontal) },
	{ nullptr, TYPE(Transform),			BMP(chart_line) },
	{ nullptr, TYPE(Camera),			BMP(camera) },
	{ nullptr, TYPE(Light),				BMP(lightbulb_off) },
	{ nullptr, TYPE(Billboard),			BMP(shape_flip_horizontal) },
	{ nullptr, TYPE(Particles),			BMP(chart_pie) },
	{ "Primitives", nullptr, nullptr, 0 },
	{ nullptr, TYPE(Cube),				BMP(cube) },
	{ "Nature", nullptr, nullptr, 0 },
	{ nullptr, TYPE(Skydome),			BMP(weather_clouds) },
	//{ nullptr, TYPE(Water),				BMP(weather_clouds) },
	{ "GUI", nullptr, nullptr, 0 },
	{ nullptr, TYPE(Label),				BMP(text_align_left) },
	//{ "Particles", nullptr, nullptr, 0 },
#ifdef VAPOR_AUDIO_OPENAL	
	{ "Audio", nullptr, nullptr, 0 },
	{ nullptr, TYPE(Source),			BMP(sound) },
	{ nullptr, TYPE(Listener),			BMP(status_online) },
#endif
	{ "Controllers", nullptr, nullptr, 0 },
	{ nullptr, TYPE(FirstPersonController), BMP(camera) },
	{ nullptr, TYPE(ThirdPersonController), BMP(camera) },
#ifdef ENABLE_SCRIPTING_LUA
	{ "Scripting", nullptr, nullptr, 0 },
	{ nullptr, TYPE(ScriptController),		BMP(shape_flip_horizontal) },
#endif
#ifdef VAPOR_PHYSICS_BULLET
	{ "Physics", nullptr, nullptr, 0 },
	{ nullptr, TYPE(CharacterController),	BMP(link) },
	{ nullptr, TYPE(BoxShape),				BMP(link) },
	{ nullptr, TYPE(MeshShape),				BMP(link) },
	{ nullptr, TYPE(CapsuleShape),			BMP(link) },
	{ nullptr, TYPE(Body),					BMP(link) },
#endif
	{ "Extra", nullptr, nullptr, 0 },
	{ nullptr, TYPE(Grid),				BMP(grid_icon_white_bg) },
	{ nullptr, TYPE(Projector),			BMP(lightbulb_off) },
};

//-----------------------------------//

void ScenePage::initIcons()
{
	// Create a new list with all the icons.
	imageList = new wxImageList(16, 16, false, 16);
	imageList->Add(wxMEMORY_BITMAP(package));

	for( size_t i = 0; i < ARRAY_SIZE(components); i++ )
	{
		const ComponentEntry& c = components[i];
		const Type* type = c.type;

		if(c.icon == nullptr) continue;

		bitmaps[type] = _wxConvertMemoryToBitmap(c.icon, c.icon_length);
		icons[type] = imageList->Add(bitmaps[type]);
	}

	treeCtrl->AssignImageList(imageList);
}

//-----------------------------------//

void ScenePage::populateEntityItemMenu(wxMenu& menu, const EntityPtr& node)
{
	menu.AppendCheckItem(ID_MenuSceneEntityVisible, "&Visible");
	menu.Check(ID_MenuSceneEntityVisible, node->isVisible() );

	const std::vector<GeometryPtr>& geo = node->getGeometry();
	
	if( !geo.empty() )
	{
		const std::vector<RenderablePtr>& rend = geo.front()->getRenderables();
		
		if( !rend.empty() )
		{
			bool state = (rend.front()->getPolygonMode() != PolygonMode::Solid);
			menu.AppendCheckItem(ID_MenuSceneEntityWireframe, "&Wireframe");
			menu.Check(ID_MenuSceneEntityWireframe, state );
		}
	}

	menu.Append(ID_MenuSceneEntityDelete, "&Delete");
	menu.Append(ID_MenuSceneEntityDuplicate, "Duplicate");

	menu.AppendSeparator();

	wxMenu* subMenu = &menu;

	for( size_t i = 0; i < ARRAY_SIZE(components); i++ )
	{
		ComponentEntry& c = components[i];
		const Type* type = c.type;
		const char* category = c.category;

		if(category != nullptr)
		{
			subMenu = new wxMenu;
			menu.Append(wxID_ANY, category, subMenu);
			continue;
		}

		wxMenuItem* item = subMenu->Append(wxID_ANY, type->name);
		if(type) item->SetBitmap( bitmaps[type], false );
	}
}

//-----------------------------------//

} } // end namespaces