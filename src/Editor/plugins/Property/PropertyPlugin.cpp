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

	selectedEntity = entity;
	propertyPage->showEntityProperties( entity );

	//Class& klass = (Class&) entity->getType();
	//klass.onFieldChanged.Connect(this, &PropertyPlugin::onFieldChanged);

	entity->onComponentAdded.Connect(this, &PropertyPlugin::onComponentChanged);
	entity->onComponentRemoved.Connect(this, &PropertyPlugin::onComponentChanged);
}

//-----------------------------------//

void PropertyPlugin::onEntityUnselect( const EntityPtr& entity )
{
	//if( selectedEntity )
	{
		selectedEntity.reset();
		propertyPage->Clear();
	}

	//Class& klass = (Class&) entity->getType();
	//klass.onFieldChanged.Disconnect(this, &PropertyPlugin::onFieldChanged);

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

void PropertyPlugin::onComponentChanged(const ComponentPtr& component)
{
	updateProperties();
}

//-----------------------------------//

void PropertyPlugin::onSceneUpdate()
{
	updateProperties();
}

//-----------------------------------//

void PropertyPlugin::updateProperties()
{
	const EntityPtr& entity = selectedEntity.lock();

	if( entity )
	{
		propertyPage->updateMemoryWatches();
		propertyPage->showEntityProperties(entity);
	}

	editor->redrawView();
}

//-----------------------------------//

} } // end namespaces