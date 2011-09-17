/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class Document;
typedef std::vector<Document*> DocumentsVector;

class DocumentManager
{
public:

	DocumentManager();
	~DocumentManager();

	// Adds a new document to the manager.
	void addDocument(Document* document);

	// Removes the document from the manager.
	void removeDocument(Document* document);

	// Gets the documents from the manager.
	GETTER(Documents, const DocumentsVector&, documents)

	// Gets the current document.
	GETTER(CurrentDocument, Document*, currentDocument)

	// Document events.
	Event1<Document*> onDocumentAdded;
	Event1<Document*> onDocumentRemoved;

	DocumentsVector documents;
	Document* currentDocument;
};

//-----------------------------------//

NAMESPACE_EDITOR_END