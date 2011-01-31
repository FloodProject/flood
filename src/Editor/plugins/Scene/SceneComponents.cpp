/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "ScenePage.h"
#include "Utilities.h"
#include "Events.h"
#include "Editor.h"
#include "EditorIcons.h"
#include "Viewframe.h"
#include "UndoManager.h"

namespace vapor { namespace editor {

//-----------------------------------//

struct ComponentEntry
{
	bool show;
	const Type& type;
	const byte* icon;
	const int icon_length;
};

#define TYPE(t) t::getStaticType()
#define BMP(s) s, sizeof(s)

static ComponentEntry components[] =
{
	{ false,TYPE(Scene),				BMP(sitemap_color) },
	{ true, TYPE(Transform),			BMP(chart_line) },
	{ true, TYPE(Model),				BMP(shape_flip_horizontal) },
	{ true, TYPE(Light),				BMP(lightbulb_off) },
	{ true, TYPE(Skydome),				BMP(weather_clouds) },
	{ true, TYPE(Camera),				BMP(camera) },
	{ true, TYPE(Label),				BMP(text_align_left) },
	{ true, TYPE(Particles),			BMP(chart_pie) },
//};

//static ComponentEntry componentsExtra[] =
//{
	{ true, TYPE(Grid),					BMP(grid_icon_white_bg) },
	{ true, TYPE(Billboard),			BMP(shape_flip_horizontal) },
	{ true, TYPE(Projector),			BMP(lightbulb_off) },
//};

#ifdef VAPOR_AUDIO_OPENAL

//static ComponentEntry componentsAudio[] =
//{
	{ true, TYPE(Source),				BMP(sound) },
	{ true, TYPE(Listener),				BMP(status_online) },
//};

#endif

//static ComponentEntry componentsControllers[] =
//{
	{ true, TYPE(FirstPersonController),BMP(camera) },
	{ true, TYPE(ThirdPersonController),BMP(camera) },

#ifdef VAPOR_SCRIPTING_LUA
	{ true, TYPE(ScriptController),		BMP(shape_flip_horizontal) },
#endif
//};

//static ComponentEntry componentsPhysics[] =
//{
#ifdef VAPOR_PHYSICS_BULLET
	{ true, TYPE(CharacterController),	BMP(link) },
	{ true, TYPE(BoxShape),				BMP(link) },
	{ true, TYPE(MeshShape),			BMP(link) },
	{ true, TYPE(CapsuleShape),			BMP(link) },
	{ true, TYPE(Body),					BMP(link) },
#endif
};

//-----------------------------------//

void ScenePage::initIcons()
{
	// Create a new list with all the icons.
	imageList = new wxImageList(16, 16, false, 16);
	imageList->Add(wxMEMORY_BITMAP(package));

	for( uint i = 0; i < VAPOR_ARRAY_SIZE(components); i++ )
	{
		const ComponentEntry& c = components[i];
		const Type* type = &c.type;
		
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

	for( uint i = 0; i < VAPOR_ARRAY_SIZE(components); i++ )
	{
		ComponentEntry& c = components[i];
		const Type& type = c.type;

		wxMenuItem* item = menu.Append(wxID_ANY, type.name);
		item->SetBitmap( bitmaps[&type], false );
	}
}

//-----------------------------------//

} } // end namespaces