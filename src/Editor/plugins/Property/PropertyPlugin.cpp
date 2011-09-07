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
	PluginMetadata metadata;
	
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
	propertyPage->SetSize(180, -1);

	wxBitmap icon = wxMEMORY_BITMAP(application_view_list);
	
	wxAuiPaneInfo pane;
	pane.Caption("Properties").Name("Properties").Right().Hide().Dock().Icon(icon);

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
	editor->getAUI()->Update();

	propertyPage->Destroy();
	propertyPage = nullptr;

	// Unsubscribe as an event listener.
	EventManager* events = editor->getEventManager();
	events->removeEventListener(this);
}

//-----------------------------------//

void PropertyPlugin::onSceneUnload( const ScenePtr& )
{
	propertyPage->reset();
}

//-----------------------------------//

void PropertyPlugin::onEntitySelect( const EntityPtr& entity )
{
	if( ClassInherits(entity->getType(), ReflectionGetType(Group)) )
		return;

	propertyPage->reset();
	propertyPage->showEntityProperties(entity);

	entity->onComponentAdded.Connect(this, &PropertyPlugin::onComponentChanged);
	entity->onComponentRemoved.Connect(this, &PropertyPlugin::onComponentChanged);
}

//-----------------------------------//

void PropertyPlugin::onEntityUnselect( const EntityPtr& entity )
{
	propertyPage->resetObject( entity.get() );

	entity->onComponentAdded.Disconnect(this, &PropertyPlugin::onComponentChanged);
	entity->onComponentRemoved.Disconnect(this, &PropertyPlugin::onComponentChanged);
}

//-----------------------------------//

void PropertyPlugin::onComponentSelect( const ComponentPtr& component )
{
	const EntityPtr& entity = component->getEntity();
	onEntitySelect(entity);
}

//-----------------------------------//

void PropertyPlugin::onComponentUnselect( const ComponentPtr& component )
{
	const EntityPtr& entity = component->getEntity();
	onEntityUnselect(entity);
}

//-----------------------------------//

void PropertyPlugin::onResourceSelect( const ResourcePtr& resource )
{
	propertyPage->showProperties( resource.get() );

	ResourceManager* rm = GetResourceManager();

	const String& path = resource->getPath();
	const String& ext = PathGetFileExtension(path);

	ResourceLoader* loader = rm->findLoader(ext);
	if( !loader ) return;

	propertyPage->showProperties(loader, false);

	ResourceProcessor* processor = PipelineFindProcessor( resource->getType() );

	if( !processor ) return;

	propertyPage->showProperties(processor, false);
}

//-----------------------------------//

void PropertyPlugin::onResourceUnselect( const ResourcePtr& resource )
{
	propertyPage->resetObject( resource.get() );
}

//-----------------------------------//

void PropertyPlugin::onComponentChanged(const ComponentPtr& component)
{
	const EntityPtr& entity = component->getEntity();
	updateProperties(entity);
}

//-----------------------------------//

void PropertyPlugin::updateProperties(const EntityPtr& entity)
{
	if( !entity ) return;

	propertyPage->reset();
	propertyPage->showEntityProperties(entity);

	editor->redrawView();
}

//-----------------------------------//

NAMESPACE_EDITOR_END

#endif