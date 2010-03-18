/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "Viewport.h"
#include "EditorIcons.h"

namespace vapor { namespace editor {

//-----------------------------------//

BEGIN_EVENT_TABLE(Viewport, wxPanel)
	// Don't remove this or you'll get undefined references.
END_EVENT_TABLE()

//-----------------------------------//

Viewport::Viewport( vapor::Engine* engine, wxWindow* parent, wxWindowID id,
				   const wxPoint& pos, const wxSize& size, long style ) 
	: wxPanel( parent, id, pos, size, style )
{
	control = new vaporControl( engine, this );

	build();
	createCamera( engine );
	onCameraTransform();
}

//-----------------------------------//

void Viewport::createCamera( Engine* engine )
{
	// Each camera will have in unique name.
	static int i = 0;

	InputManager* const im = engine->getInputManager();
	render::Device* const rd = engine->getRenderDevice();
	
	// Create a new first-person camera for our viewport.
	// By default it will be in perspective projection.
	camera.reset( new FirstPersonCamera(im, rd) );
	cameraTransform.reset( new Transform() );

	// Generate a new unique name and increment it.
	std::string name( "EditorCamera" + num_to_str(i++) );
	
	cameraNode.reset( new Node(name) );
	cameraNode->addComponent( cameraTransform );
	cameraNode->addComponent( camera );
	cameraNode->setTag( EditorTags::EditorOnly, true );
	
	// Subscribe to the transform events.
	cameraTransform->onTransform +=
		fd::bind( &Viewport::onCameraTransform, this );
}

//-----------------------------------//

void Viewport::onCameraTransform()
{
	// We need to switch to a neutral locale or else the text
	// conversion will lead to different results depending on
	// each machine due to different locale settings.
	LocaleSaveRestore c;
	
	const Vector3& pos = cameraTransform->getPosition();
	char str[32];

	float_to_str( str, pos.x );
	txt_X->ChangeValue( str );
	
	float_to_str( str, pos.y );
	txt_Y->ChangeValue( str );
	
	float_to_str( str, pos.z );
	txt_Z->ChangeValue( str );

	control->flagRedraw();
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
	cameraTransform->setPosition( pos );
}

//-----------------------------------//

void Viewport::onTextEnter( wxCommandEvent& )
{
	updatePosition();
}

//-----------------------------------//

const short BAR_HEIGHT = 20;

void Viewport::build()
{
	wxBoxSizer* sizer = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticText* lbl_X = new wxStaticText( this, wxID_ANY, wxT("X:"),
		wxDefaultPosition, wxDefaultSize, 0 );
	lbl_X->Wrap( -1 );
	sizer->Add( lbl_X, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );
	
	const wxString includes( "0123456789." );

	wxTextValidator validatorX(wxFILTER_INCLUDE_CHAR_LIST, &X);
	validatorX.SetCharIncludes( includes );
	txt_X = new wxTextCtrl( this, wxID_ANY, wxT("0.0"),
		wxDefaultPosition, wxSize( 50, BAR_HEIGHT ),
		wxTE_CENTRE | wxTE_PROCESS_ENTER, validatorX);

	txt_X->Bind(wxEVT_COMMAND_TEXT_ENTER, &Viewport::onTextEnter, this);
	sizer->Add( txt_X, 0, wxALIGN_CENTER_VERTICAL, 5 );

	wxStaticText* lbl_Y = new wxStaticText( this, wxID_ANY, wxT("Y:"),
		wxDefaultPosition, wxDefaultSize, 0 );
	lbl_Y->Wrap( -1 );
	sizer->Add( lbl_Y, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );

	wxTextValidator validatorY(wxFILTER_INCLUDE_CHAR_LIST, &Y);
	validatorY.SetCharIncludes( includes );
	txt_Y = new wxTextCtrl( this, wxID_ANY, wxT("0.0"),
		wxDefaultPosition, wxSize( 50, BAR_HEIGHT ),
		wxTE_CENTRE | wxTE_PROCESS_ENTER, validatorY);

	txt_Y->Bind(wxEVT_COMMAND_TEXT_ENTER, &Viewport::onTextEnter, this);
	sizer->Add( txt_Y, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	wxStaticText* lbl_Z = new wxStaticText( this, wxID_ANY, wxT("Z:"),
		wxDefaultPosition, wxDefaultSize, 0 );
	lbl_Z->Wrap( -1 );
	sizer->Add( lbl_Z, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );
	
	wxTextValidator validatorZ(wxFILTER_INCLUDE_CHAR_LIST, &Z);
	validatorZ.SetCharIncludes( includes );
	txt_Z = new wxTextCtrl( this, wxID_ANY, wxT("0.0"), wxDefaultPosition,
		wxSize( 50, BAR_HEIGHT ), wxTE_CENTRE | wxTE_PROCESS_ENTER | wxBORDER_THEME,
		validatorZ);

	txt_Z->Bind(wxEVT_COMMAND_TEXT_ENTER, &Viewport::onTextEnter, this);
	sizer->Add( txt_Z, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	wxStaticLine* m_staticline2 = new wxStaticLine( 
		this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	sizer->Add( m_staticline2, 0, wxEXPAND|wxALL, 5 );
	
	//wxString choice_ViewChoices[] = 
	// { "Free", wxT("Top"), wxT("Down"), wxT("Left"), wxT("Right") };
	//choice_View = new wxChoice( this, wxID_ANY, wxDefaultPosition,
	// wxSize( -1, BAR_HEIGHT-1 ), 
	//	VAPOR_ARRAY_SIZE(choice_ViewChoices), choice_ViewChoices, 0 );
	//choice_View->SetSelection( 0 );
	//sizer->Add( choice_View, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );

	wxStaticText* lbl_Speed = new wxStaticText( this, wxID_ANY, wxT("Speed:") );
	lbl_Speed->Wrap( -1 );
	sizer->Add( lbl_Speed, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5 );

	wxSpinCtrlDouble* spn_CameraSpeed = new wxSpinCtrlDouble( this, wxID_ANY,
		wxEmptyString, wxDefaultPosition, wxSize( 60, BAR_HEIGHT ) );
	sizer->Add( spn_CameraSpeed, 0, wxALIGN_CENTER_VERTICAL, 5 );

	btn_Wireframe = new wxBitmapButton( this, wxID_ANY, wxMEMORY_BITMAP(grid_icon_small),
		wxDefaultPosition, wxSize( -1, BAR_HEIGHT-1 ), wxBU_AUTODRAW );
	sizer->Add( btn_Wireframe, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5 );
	
	btn_Textures = new wxBitmapButton( this, wxID_ANY, wxMEMORY_BITMAP(map_small),
		wxDefaultPosition, wxSize( -1, BAR_HEIGHT-1 ), wxBU_AUTODRAW );
	sizer->Add( btn_Textures, 0, wxALIGN_CENTER_VERTICAL, 5 );

	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );

	mainSizer->Add( control, 1, wxEXPAND|wxALL );
	mainSizer->Add( sizer, 0, wxEXPAND|wxALL );

	this->SetSizer( mainSizer );
	this->Layout();
}

//-----------------------------------//

} } // end namespaces
