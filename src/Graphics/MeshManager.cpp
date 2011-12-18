/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Graphics/MeshManager.h"
#include "Resources/ResourceManager.h"
#include "Graphics/GL.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

MeshManager::MeshManager()
{
	ResourceManager* res = GetResourceManager();
	res->onResourceLoaded.Connect( this, &MeshManager::onLoaded );
	res->onResourceRemoved.Connect( this, &MeshManager::onUnloaded );
	res->onResourceReloaded.Connect( this, &MeshManager::onReloaded );
}

//-----------------------------------//

MeshManager::~MeshManager()
{
	ResourceManager* res = GetResourceManager();
	res->onResourceLoaded.Disconnect( this, &MeshManager::onLoaded );
	res->onResourceRemoved.Disconnect( this, &MeshManager::onUnloaded );
	res->onResourceReloaded.Disconnect( this, &MeshManager::onReloaded );
}

//-----------------------------------//

void MeshManager::update( float )
{

}

//-----------------------------------//

void MeshManager::removeMesh(Mesh* mesh)
{
	MeshRenderablesMap::iterator it = meshRenderables.find(mesh);
	
	if( it == meshRenderables.end() )
		return;

	meshRenderables.erase(it);
}

//-----------------------------------//

bool MeshManager::getMeshRenderables( Mesh* mesh, RenderablesVector& rends )
{
	if( !mesh ) return false;

	MeshRenderablesMap::iterator it = meshRenderables.find(mesh);
	
	if( it == meshRenderables.end() )
		return false;

	rends = it->second;
	
	return true;
}

//-----------------------------------//

void MeshManager::onLoaded( const ResourceEvent& event )
{
	Resource* resource = event.handle.Resolve();

	if( resource->getResourceGroup() != ResourceGroup::Meshes )
		return;

	Mesh* mesh = (Mesh*) resource;

	if( meshRenderables.find(mesh) != meshRenderables.end() )
		return;

	LogDebug("Building mesh: %s (refs: %d)", mesh->getPath().c_str(), mesh->references);
	
	build(mesh);
}

//-----------------------------------//

void MeshManager::onUnloaded( const ResourceEvent& event )
{
	Resource* resource = event.handle.Resolve();

	if( resource->getResourceGroup() != ResourceGroup::Meshes )
		return;

	Mesh* mesh = (Mesh*) resource;

	if( meshRenderables.find(mesh) == meshRenderables.end() )
		return;

	LogDebug( "Removing mesh '%s'", mesh->getPath().c_str() );

	removeMesh(mesh);
}

//-----------------------------------//

void MeshManager::onReloaded( const ResourceEvent& event )
{
#if 0
	Resource* resource = event.handle.Resolve();

	if( resource->getResourceGroup() != ResourceGroup::Meshes )
		return;

	Mesh* mesh = (Mesh*) resource;

	if( meshRenderables.find(mesh) == meshRenderables.end() )
		return;

	LogDebug( "Reloading mesh '%s'", mesh->getPath().c_str() );

	Mesh* cmesh = meshRenderables[mesh];
	mesh->setMesh(mesh);
#endif
}

//-----------------------------------//

NAMESPACE_ENGINE_END