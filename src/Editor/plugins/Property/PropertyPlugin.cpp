/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "PropertyPlugin.h"
#include "PropertyPage.h"
#include "Editor.h"
#include "EventManager.h"
#include "EditorIcons.h"
#include "Core/Reflection.h"
#include "Core/Utilities.h"
#include "DocumentWindow.h"
#include "Pipeline/ResourceProcessor.h"

#ifdef ENABLE_PLUGIN_PROPERTY

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(PropertyPlugin, EditorPlugin)
REFLECT_CLASS_END()

//-----------------------------------//

PropertyPlugin::PropertyPlugin()
	: propertyPage(nullptr)
{ }

//-----------------------------------//

PluginMetadata PropertyPlugin::getMetadata()
{
	static PluginMetadata metadata;
	
	metadata.name = "Property";
	metadata.description = "Provides a page with object properties.";
	metadata.author = "triton";
	metadata.version = "1.0";
	metadata.priority = 300;

	return metadata;
}

//-----------------------------------//

void PropertyPlugin::onPluginEnable()
{
	propertyPage = new PropertyPage(editor);

	wxBitmap icon = wxMEMORY_BITMAP(application_view_list);
	
	wxAuiPaneInfo pane;
	pane.Caption("Properties").Name("Properties").Right().Hide().Dock().Icon(icon);
	//pane.MinSize( wxSize(200, 200) );
	pane.dock_proportion = 0;

	editor->getAUI()->AddPane(propertyPage, pane);
	editor->getAUI()->Update();

	// Subscribe as an event listener.
	EventManager* events = editor->getEventManager();
	events->addEventListener(this);
}

//-----------------------------------//

void PropertyPlugin::onPluginDisable()
{
	editor->getAUI()->DetachPane(propertyPage);
	//editor->getAUI()->Update();

	propertyPage->Destroy();
	propertyPage = nullptr;

	// Unsubscribe as an event listener.
	EventManager* events = editor->getEventManager();
	events->removeEventListener(this);
}

//-----------------------------------//

static ResourceLoader* GetResourceLoader(Resource* resource)
{
	if( !resource ) return nullptr;

	ResourceManager* res = GetResourceManager();

	const Path& path = resource->getPath();
	const String& ext = PathGetFileExtension(path);

	ResourceLoader* loader = res->findLoader(ext);
	return loader;
}

//-----------------------------------//

void PropertyPlugin::onResourceSelect( const ResourcePtr& resource )
{
	return;

	PropertyPage* page = propertyPage;

	wxFoldPanel foldPanel = page->AddFoldPanel("Resource");
	foldPanel.GetItem()->GetCaptionBar()->Hide();

	PropertyGrid* grid = page->createPropertyGrid( foldPanel.GetParent() );
	//propertyPage->showProperties( grid, resource.get() );

	ResourceLoader* loader = GetResourceLoader(resource.get());
	if( !loader ) return;

	propertyPage->showProperties(grid, loader, false);

	ResourceProcessor* processor = PipelineFindProcessor( resource->getType() );
	if( !processor ) return;

	propertyPage->showProperties(grid, processor, false);
}

//-----------------------------------//

void PropertyPlugin::onResourceUnselect( const ResourcePtr& resource )
{
	propertyPage->resetObject( resource.get() );
}

//-----------------------------------//

NAMESPACE_EDITOR_END

#endif