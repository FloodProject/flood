/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "PCH.h"
#include "Viewport.h"
#include "EditorIcons.h"
#include <vapor/Utilities.h>

namespace vapor { namespace editor {

//-----------------------------------//

BEGIN_EVENT_TABLE(Viewport, wxPanel)
	//
END_EVENT_TABLE()

//-----------------------------------//

Viewport::Viewport( vapor::Engine* engine, wxWindow* parent, wxWindowID id,
				   const wxPoint& pos, const wxSize& size, long style ) 
	: wxPanel( parent, id, pos, size, style ), engine( engine )
{
	init();
	build();
	createCamera();
	onCameraTransform();
}

//-----------------------------------//

Viewport::~Viewport()
{

}

//-----------------------------------//

void Viewport::init()
{
	int attribs[] = 
	{
		WX_GL_RGBA,
		WX_GL_DOUBLEBUFFER,
		WX_GL_DEPTH_SIZE, 32,
		WX_GL_SAMPLE_BUFFERS, 1,
		WX_GL_SAMPLES, 0,
		0,
	};

	vaporCtrl = new vaporControl( engine, this/*, wxID_ANY, attribs*/ );
}

//-----------------------------------//

void Viewport::createCamera()
{
	cameraNode.reset( new Node( "MainCamera" ) );
	camera.reset( new FirstPersonCamera( engine->getInputManager(), engine->getRenderDevice() ) );
	cameraNode->addComponent( TransformPtr( new Transform() ) );
	cameraNode->addComponent( camera );
	// TODO: hardcoded camera start position...
	cameraNode->getTransform()->translate( 0.0f, 20.0f, -65.0f );
	engine->getSceneManager()->add( cameraNode );

	vaporCtrl->setCamera( camera );

	transform = cameraNode->getTransform();
	transform->onTransform += fd::bind( &Viewport::onCameraTransform, this );
}

//-----------------------------------//

const short BAR_HEIGHT = 19;

void Viewport::build()
{
	wxBoxSizer* sizer = new wxBoxSizer( wxHORIZONTAL );
	
	lbl_X = new wxStaticText( this, wxID_ANY, wxT("X:"), wxDefaultPosition, wxDefaultSize, 0 );
	lbl_X->Wrap( -1 );
	sizer->Add( lbl_X, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );
	
	const wxString includes( "0123456789." );

	wxTextValidator validatorX(wxFILTER_INCLUDE_CHAR_LIST, &X);
	validatorX.SetCharIncludes( includes );
	txt_X = new wxTextCtrl( this, wxID_ANY, wxT("0.0"), wxDefaultPosition, wxSize( 50, BAR_HEIGHT ), wxTE_CENTRE | wxTE_PROCESS_ENTER,
		validatorX);
	
	txt_X->Bind(wxEVT_COMMAND_TEXT_UPDATED, &Viewport::onText, this);
	txt_X->Bind(wxEVT_KILL_FOCUS, &Viewport::onKillFocus, this);
	txt_X->Bind(wxEVT_COMMAND_TEXT_ENTER, &Viewport::onTextEnter, this);

	sizer->Add( txt_X, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	lbl_Y = new wxStaticText( this, wxID_ANY, wxT("Y:"), wxDefaultPosition, wxDefaultSize, 0 );
	lbl_Y->Wrap( -1 );
	sizer->Add( lbl_Y, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );
	
	wxTextValidator validatorY(wxFILTER_INCLUDE_CHAR_LIST, &Y);
	validatorY.SetCharIncludes( includes );
	txt_Y = new wxTextCtrl( this, wxID_ANY, wxT("0.0"), wxDefaultPosition, wxSize( 50, BAR_HEIGHT ), wxTE_CENTRE | wxTE_PROCESS_ENTER,
		validatorY);
	
	txt_Y->Bind(wxEVT_COMMAND_TEXT_UPDATED, &Viewport::onText, this);
	txt_Y->Bind(wxEVT_KILL_FOCUS, &Viewport::onKillFocus, this);
	txt_Y->Bind(wxEVT_COMMAND_TEXT_ENTER, &Viewport::onTextEnter, this);

	sizer->Add( txt_Y, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	lbl_Z = new wxStaticText( this, wxID_ANY, wxT("Z:"), wxDefaultPosition, wxDefaultSize, 0 );
	lbl_Z->Wrap( -1 );
	sizer->Add( lbl_Z, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );
	
	wxTextValidator validatorZ(wxFILTER_INCLUDE_CHAR_LIST, &Z);
	validatorZ.SetCharIncludes( includes );
	txt_Z = new wxTextCtrl( this, wxID_ANY, wxT("0.0"), wxDefaultPosition, wxSize( 50, BAR_HEIGHT ), wxTE_CENTRE | wxTE_PROCESS_ENTER,
		validatorZ);
	
	txt_Z->Bind(wxEVT_COMMAND_TEXT_UPDATED, &Viewport::onText, this);
	txt_Z->Bind(wxEVT_KILL_FOCUS, &Viewport::onKillFocus, this);
	txt_Z->Bind(wxEVT_COMMAND_TEXT_ENTER, &Viewport::onTextEnter, this);

	sizer->Add( txt_Z, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticline2 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	sizer->Add( m_staticline2, 0, wxEXPAND|wxALL, 5 );
	
	wxString choice_ViewChoices[] = { "Free", wxT("Top"), wxT("Down"), wxT("Left"), wxT("Right") };
	choice_View = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxSize( -1, BAR_HEIGHT ), 
		VAPOR_ARRAY_SIZE(choice_ViewChoices), choice_ViewChoices, 0 );
	choice_View->SetSelection( 0 );
	sizer->Add( choice_View, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );
	
	btn_Wireframe = new wxBitmapButton( this, wxID_ANY, wxMEMORY_BITMAP(grid_icon_small), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	sizer->Add( btn_Wireframe, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	btn_Textures = new wxBitmapButton( this, wxID_ANY, wxMEMORY_BITMAP(map_small), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	sizer->Add( btn_Textures, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );

	mainSizer->Add( vaporCtrl, 1, wxEXPAND|wxALL );
	mainSizer->Add( sizer, 0, wxEXPAND|wxALL );

	this->SetSizer( mainSizer );
	this->Layout();
}

//-----------------------------------//

#define CONVERT_STR(n)												\
	sprintf( str, "%#.*f", CAM_STR_PRECISION, n );					\
	while((c = strchr(str, ','))) *c = '.';							\

const byte CAM_STR_PRECISION = 2;

void Viewport::onCameraTransform()
{
	const math::Vector3& pos = transform->getPosition();
	
	char str[20]; char* c;
	
	CONVERT_STR( pos.x )
	txt_X->ChangeValue( str );
	
	CONVERT_STR( pos.y )
	txt_Y->ChangeValue( str );
	
	CONVERT_STR( pos.z )
	txt_Z->ChangeValue( str );
}

//-----------------------------------//

void Viewport::updatePosition()
{
	std::string str( txt_X->GetValue() );
	float X = str_to_num<float>( str );
	
	str.assign( txt_Y->GetValue() );
	float Y = str_to_num<float>( str );
	
	str.assign( txt_Z->GetValue() );
	float Z = str_to_num<float>( str );

	math::Vector3 pos( X, Y, Z );
	transform->setPosition( pos );
}

//-----------------------------------//

void Viewport::onText( wxCommandEvent& event )
{

}

//-----------------------------------//

void Viewport::onTextEnter( wxCommandEvent& event )
{
	updatePosition();
}

//-----------------------------------//

void Viewport::onKillFocus( wxFocusEvent& event )
{
	updatePosition();
	event.Skip();
}

//-----------------------------------//

} } // end namespaces
