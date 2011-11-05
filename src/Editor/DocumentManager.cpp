/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "DocumentManager.h"
#include "Document.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

DocumentManager::DocumentManager()
	: currentDocument(nullptr)
{
}

//-----------------------------------//

DocumentManager::~DocumentManager()
{
	while( !documents.empty() )
	{
		Document* document = documents.front();
		removeDocument(document);
	}
}

//-----------------------------------//

void DocumentManager::addDocument(Document* document)
{
	if( !document ) return;
	documents.push_back(document);

	onDocumentAdded(document);
}

//-----------------------------------//

void DocumentManager::removeDocument(Document* document)
{
	DocumentsVector::iterator it = std::find(documents.begin(), documents.end(), document);
	if(it == documents.end()) return;

	LogInfo("Removing document: %s", document->getPath().c_str());

	document->onDocumentUnselect();
	document->onDocumentDestroy();

	onDocumentRemoved(document);

	documents.erase(it);

	if(currentDocument == document)
	{
		// The current document changed.
		currentDocument = nullptr;
	}

	Deallocate(document);
}

//-----------------------------------//

NAMESPACE_EDITOR_END