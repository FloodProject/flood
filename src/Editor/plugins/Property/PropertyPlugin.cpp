/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "PropertyPlugin.h"
#include "PropertyPage.h"
#include "Editor.h"
#include "Events.h"
#include "EditorIcons.h"
#include "Utilities.h"
#include "Pipeline/ResourceProcessor.h"

namespace vapor { namespace editor {

//-----------------------------------//

PropertyPlugin::PropertyPlugin( EditorFrame* frame )
	: Plugin(frame)
	, propertyPage(nullptr)
{ }

//-----------------------------------//

PluginMetadata PropertyPlugin::getMetadata()
{
	PluginMetadata metadata;
	
	metadata.name = "Property";
	metadata.description = "Provides a page with object properties";
	metadata.author = "triton";
	metadata.version = "1.0";

	return metadata;
}

//-----------------------------------//

void PropertyPlugin::onPluginEnable()
{
	propertyPage = new PropertyPage(editor);
	propertyPage->SetSize(220, 400);

	wxBitmap icon = wxMEMORY_BITMAP(application_view_list);
	
	wxAuiPaneInfo pane;
	pane.Caption("Properties").Right().Dock().Icon(icon);

	editor->getAUI()->AddPane(propertyPage, pane);
	editor->getAUI()->Update();

	// Subscribe as an event listener.
	Events* events = editor->getEventManager();
	events->addEventListener(this);
}

//-----------------------------------//

void PropertyPlugin::onPluginDisable()
{
	editor->getAUI()->DetachPane(propertyPage);
	editor->getAUI()->Update();

	delete propertyPage;
	propertyPage = nullptr;

	// Unsubscribe as an event listener.
	Events* events = editor->getEventManager();
	events->removeEventListener(this);
}

//-----------------------------------//

void PropertyPlugin::onEntitySelect( const EntityPtr& entity )
{
	if( entity->getType().inherits<Group>() )
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
	const EntityPtr& entity = component->getEntity()->getShared();
	onEntitySelect(entity);
}

//-----------------------------------//

void PropertyPlugin::onComponentUnselect( const ComponentPtr& component )
{
	const EntityPtr& entity = component->getEntity()->getShared();
	onEntityUnselect(entity);
}

//-----------------------------------//

void PropertyPlugin::onResourceSelect( const ResourcePtr& resource )
{
	propertyPage->showProperties( resource.get() );

	ResourceManager* rm = GetResourceManager();

	const std::string& path = resource->getPath();
	const std::string& ext = PathUtils::getExtension(path);

	ResourceLoader* loader = rm->findLoader(ext);
	if( !loader ) return;

	propertyPage->showProperties(loader, false);

	ResourceProcessor* processor =
		ResourceProcessor::findProcessor( resource->getType() );

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
	const EntityPtr& entity = component->getEntity()->getShared();
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

void PropertyPlugin::onSceneLoad( const ScenePtr& scene )
{
	propertyPage->reset();
}

//-----------------------------------//

} } // end namespaces