/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Extensions/Resources/ResourcesPlugin.h"
#include "Extensions/Resources/ResourcesBrowser.h"
#include "Editor.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

static wxString GetReadableName(wxString name)
{
	if( name.IsEmpty() ) return wxEmptyString;

	name.Replace("_", " ");

	wxString format;
	format.Append(wxUniChar(toupper(name[0])));

	// Add spaces between words.
	for( size_t i = 1; i < name.Len(); i++ )
	{
		format.Append(name[i]);

		if( i < name.Len()-1 )
		{
			if( !isspace(name[i]) && islower(name[i]) && isupper(name[i+1]) )
				format.Append(" ");
		}
	}

	return format;
}

//-----------------------------------//

bool ReflectionIsResourceHandle(const Field* field)
{
	Type* type = field->type;

	bool isClass = ReflectionIsComposite(type);
	bool isResource = isClass && ClassInherits((Class*) type, ReflectionGetType(Resource));
	
	return isResource && FieldIsHandle(field);
}

//-----------------------------------//

static ResourceHandle GetResourceUsingBrowser( ResourceGroup::Enum group )
{
	ResourcesPlugin* rp = GetPlugin<ResourcesPlugin>();
	ResourcesBrowser* rb = rp->resourcesBrowser;
	
	rb->Close();
	rb->enableSelection();

	rb->selectGroup(group);
	int index = rb->ShowModal();
	rb->disableSelection();

	if( index < 0 ) return HandleInvalid;
	
	wxString selection = rb->getListCtrl()->GetItemText(index);
	String path = (String) selection.c_str();

	return GetResourceManager()->loadResource(path);
}

//-----------------------------------//
#if 0
static ResourceHandle GetResourceUsingFile( ResourceGroup::Enum group )
{
	wxFileDialog fd( &GetEditor(), wxFileSelectorPromptStr,
			wxEmptyString, wxEmptyString, "Resource files | *.*",
			wxFD_DEFAULT_STYLE | wxFD_FILE_MUST_EXIST );

	if( fd.ShowModal() != wxID_OK )
		return ResourceHandle(HandleInvalid);

	String path( fd.GetPath() );
	path = PathNormalize(path);
	
	ResourceManager* res = GetResourceManager();
	return res->loadResource( PathGetFile(path) );
}
#endif
//-----------------------------------//

class ResourceProperty : public wxLongStringProperty
{
public:

	ResourceProperty(const String& name)
		: wxLongStringProperty(name, wxPG_LABEL)
	{}

	virtual bool OnButtonClick( wxPropertyGrid* propGrid, wxString& value ) OVERRIDE
	{
		PropertyData* data = (PropertyData*) GetClientObject();
		Class* resourceClass = (Class*) data->field->type;
		
		ResourceLoader* resourceLoader = GetResourceManager()->findLoaderByClass(resourceClass);
		if( !resourceLoader ) return false;

		ResourceHandle resource = GetResourceUsingBrowser( resourceLoader->getResourceGroup() );
		if( !resource ) return false;

		Resource* res = resource.Resolve();
		SetValueInEvent( PathGetFile(res->getPath()) );

		return true;
	}
};

//-----------------------------------//

NAMESPACE_EDITOR_END