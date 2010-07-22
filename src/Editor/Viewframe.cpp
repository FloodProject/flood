/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "Viewframe.h"
#include "EditorIcons.h"
#include "RenderWindow.h"
#include "RenderControl.h"

namespace vapor { namespace editor {

//-----------------------------------//

enum
{
	ID_CameraSpeed = 5649,
};

//-----------------------------------//

BEGIN_EVENT_TABLE(Viewframe, wxPanel)
	//EVT_SPINCTRLDOUBLE(ID_CameraSpeed, Viewframe::onCameraSpeedSpin)
END_EVENT_TABLE()

//-----------------------------------//

Viewframe::Viewframe( wxWindow* parent, wxWindowID id,
				   const wxPoint& pos, const wxSize& size, long style ) 
	: wxPanel( parent, id, pos, size, style )
{
	control = new RenderControl( this );
	build();
}

//-----------------------------------//

Viewframe::~Viewframe()
{
	control->stopFrameLoop();
	control->Destroy();
}

//-----------------------------------//

Viewport* Viewframe::createViewport( NodePtr node )
{
	assert( node != nullptr );

	// Add a new viewport to the window.
	CameraPtr camera = node->getComponent<Camera>("Camera");
	assert( camera != nullptr );

	Window* window = control->getRenderWindow();
	viewport = window->addViewport( camera );

	// Subscribe to the camera transform events.
	TransformPtr transform = node->getTransform();
	transform->onTransform += fd::bind( &Viewframe::onCameraTransform, this );
	onCameraTransform();

	FirstPersonCameraPtr fps = std::static_pointer_cast<FirstPersonCamera>(camera);
	double sens = fps->getMoveSensivity();
	
	assert( spn_CameraSpeed != nullptr );
	spn_CameraSpeed->SetValue( sens );

	return viewport;
}

//-----------------------------------//

void Viewframe::flagRedraw()
{
	if( control )
		control->flagRedraw();
}

//-----------------------------------//

void Viewframe::onCameraSpeedSpin( wxSpinDoubleEvent& event )
{
	double value = event.GetValue();

	assert( viewport != nullptr );
	CameraPtr camera( viewport->getCamera() );
	
	FirstPersonCameraPtr fps =
		std::static_pointer_cast<FirstPersonCamera>(camera);
	
	fps->setMoveSensivity( value );
}

//-----------------------------------//

void Viewframe::onCameraTransform()
{
	// We need to switch to a neutral locale or else the text
	// conversion will lead to different results depending on
	// each machine due to different locale settings.
	LocaleSaveRestore c;
	
	NodePtr camera( viewport->getCamera()->getNode() );
	TransformPtr transform = camera->getTransform();
	
	const Vector3& pos = transform->getPosition();
	char str[32];

	String::fromFloat( str, pos.x );
	txt_X->ChangeValue( str );
	
	String::fromFloat( str, pos.y );
	txt_Y->ChangeValue( str );
	
	String::fromFloat( str, pos.z );
	txt_Z->ChangeValue( str );

	control->flagRedraw();
}

//-----------------------------------//

void Viewframe::updatePosition()
{
	std::string str( txt_X->GetValue() );
	float X = String::toNumber<float>( str );
	
	str.assign( txt_Y->GetValue() );
	float Y = String::toNumber<float>( str );
	
	str.assign( txt_Z->GetValue() );
	float Z = String::toNumber<float>( str );

	NodePtr camera( viewport->getCamera()->getNode() );
	TransformPtr transform = camera->getTransform();
	transform->setPosition( Vector3(X, Y, Z) );
}

//-----------------------------------//

void Viewframe::onTextEnter( wxCommandEvent& )
{
	updatePosition();
}

//-----------------------------------//

const short BAR_HEIGHT = 20;

void Viewframe::build()
{
	wxPanel* panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

	wxBoxSizer* sizer = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticText* lbl_X = new wxStaticText( panel, wxID_ANY, wxT("X:"),
		wxDefaultPosition, wxDefaultSize, 0 );
	lbl_X->Wrap( -1 );
	sizer->Add( lbl_X, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );
	
	const wxString includes( "0123456789." );

	wxTextValidator validatorX(wxFILTER_INCLUDE_CHAR_LIST, &X);
	validatorX.SetCharIncludes( includes );
	txt_X = new wxTextCtrl( panel, wxID_ANY, wxT("0.0"),
		wxDefaultPosition, wxSize( 50, BAR_HEIGHT ),
		wxTE_CENTRE | wxTE_PROCESS_ENTER, validatorX);

	txt_X->Bind(wxEVT_COMMAND_TEXT_ENTER, &Viewframe::onTextEnter, this);
	sizer->Add( txt_X, 0, wxALIGN_CENTER_VERTICAL, 5 );

	wxStaticText* lbl_Y = new wxStaticText( panel, wxID_ANY, wxT("Y:"),
		wxDefaultPosition, wxDefaultSize, 0 );
	lbl_Y->Wrap( -1 );
	sizer->Add( lbl_Y, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );

	wxTextValidator validatorY(wxFILTER_INCLUDE_CHAR_LIST, &Y);
	validatorY.SetCharIncludes( includes );
	txt_Y = new wxTextCtrl( panel, wxID_ANY, wxT("0.0"),
		wxDefaultPosition, wxSize( 50, BAR_HEIGHT ),
		wxTE_CENTRE | wxTE_PROCESS_ENTER, validatorY);

	txt_Y->Bind(wxEVT_COMMAND_TEXT_ENTER, &Viewframe::onTextEnter, this);
	sizer->Add( txt_Y, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	wxStaticText* lbl_Z = new wxStaticText( panel, wxID_ANY, wxT("Z:"),
		wxDefaultPosition, wxDefaultSize, 0 );
	lbl_Z->Wrap( -1 );
	sizer->Add( lbl_Z, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );
	
	wxTextValidator validatorZ(wxFILTER_INCLUDE_CHAR_LIST, &Z);
	validatorZ.SetCharIncludes( includes );
	txt_Z = new wxTextCtrl( panel, wxID_ANY, wxT("0.0"), wxDefaultPosition,
		wxSize( 50, BAR_HEIGHT ), wxTE_CENTRE | wxTE_PROCESS_ENTER | wxBORDER_THEME,
		validatorZ);

	txt_Z->Bind(wxEVT_COMMAND_TEXT_ENTER, &Viewframe::onTextEnter, this);
	sizer->Add( txt_Z, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	wxStaticLine* m_staticline2 = new wxStaticLine( 
		panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	sizer->Add( m_staticline2, 0, wxEXPAND|wxALL, 5 );

	wxStaticText* lbl_Speed = new wxStaticText( panel, wxID_ANY, wxT("Speed:") );
	lbl_Speed->Wrap( -1 );
	sizer->Add( lbl_Speed, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5 );

	spn_CameraSpeed = new wxSpinCtrlDouble( panel, ID_CameraSpeed,
		wxEmptyString, wxDefaultPosition, wxSize( 60, BAR_HEIGHT ) );
	spn_CameraSpeed->Bind(wxEVT_COMMAND_SPINCTRLDOUBLE_UPDATED, &Viewframe::onCameraSpeedSpin, this);
	spn_CameraSpeed->SetRange(0.0f, 1000.0f);
	spn_CameraSpeed->SetIncrement(10.0f);
	sizer->Add( spn_CameraSpeed, 0, wxALIGN_CENTER_VERTICAL, 5 );

	btn_Wireframe = new wxBitmapButton( panel, wxID_ANY, wxMEMORY_BITMAP(grid_icon_small),
		wxDefaultPosition, wxSize( -1, BAR_HEIGHT-1 ), wxBU_AUTODRAW );
	sizer->Add( btn_Wireframe, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5 );
	
	btn_Textures = new wxBitmapButton( panel, wxID_ANY, wxMEMORY_BITMAP(map_small),
		wxDefaultPosition, wxSize( -1, BAR_HEIGHT-1 ), wxBU_AUTODRAW );
	sizer->Add( btn_Textures, 0, wxALIGN_CENTER_VERTICAL, 5 );

	wxString choice_ViewChoices[] = 
	 { "Free", wxT("Top"), wxT("Down"), wxT("Left"), wxT("Right") };
	choice_View = new wxChoice( panel, wxID_ANY, wxDefaultPosition,
	 wxSize( -1, BAR_HEIGHT-1 ), 
		VAPOR_ARRAY_SIZE(choice_ViewChoices), choice_ViewChoices, 0 );
	choice_View->SetSelection( 0 );
	sizer->Add( choice_View, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );

	panel->SetSizerAndFit( sizer );

	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );

	mainSizer->Add( control, 1, wxEXPAND );
	mainSizer->Add( panel, 0, wxEXPAND|wxTOP, 2 );

	SetSizerAndFit( mainSizer );
	Layout();
}

//-----------------------------------//

} } // end namespaces
