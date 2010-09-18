/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "CameraControls.h"
#include "Viewframe.h"
#include "EditorIcons.h"

namespace vapor { namespace editor {

//-----------------------------------//

CameraControls::CameraControls( wxWindow* parent, wxWindowID id,
					 const wxPoint& pos, const wxSize& size, long style ) 
	: wxPanel(parent, id, pos, size, style)
	, viewframe(nullptr)
{
	buildControls();

	viewframe = (Viewframe*) parent;
	assert( viewframe != nullptr );

	TransformPtr transCamera = getCameraTransform();

	transCamera->onTransform +=
		fd::bind( &CameraControls::onCameraTransform, this );
	
	onCameraTransform();
	updateCameraSpeedSpin();
}

//-----------------------------------//

CameraControls::~CameraControls()
{
	TransformPtr transCamera = getCameraTransform();

	transCamera->onTransform -=
		fd::bind( &CameraControls::onCameraTransform, this );
}

//-----------------------------------//

TransformPtr CameraControls::getCameraTransform() const
{
	View* view = viewframe->getView();
	assert( view != nullptr );

	CameraPtr camera = view->getCamera();
	assert( camera != nullptr );

	NodePtr nodeCamera = camera->getNode();
	assert( nodeCamera != nullptr );

	TransformPtr transCamera = nodeCamera->getTransform();
	assert( transCamera != nullptr );

	return transCamera;
}

//-----------------------------------//

void CameraControls::onCameraSpeedSpin( wxSpinDoubleEvent& event )
{
	double value = event.GetValue();

	View* view = viewframe->getView();
	
	CameraPtr camera( view->getCamera() );
	assert( camera != nullptr );

	NodePtr nodeCamera = camera->getNode();
	assert( nodeCamera != nullptr );

	CameraControllerPtr cameraController =
		nodeCamera->getTypedComponent<CameraController>();
	
	cameraController->setMoveSensivity( value );
}

//-----------------------------------//

void CameraControls::updateCameraSpeedSpin()
{
	View* view = viewframe->getView();
	
	CameraPtr camera( view->getCamera() );
	assert( camera != nullptr );

	NodePtr nodeCamera = camera->getNode();
	assert( nodeCamera != nullptr );

	CameraControllerPtr cameraController =
		nodeCamera->getTypedComponent<CameraController>();

	double cameraMoveSensivity = cameraController->getMoveSensivity();
	
	assert( spinCameraSpeed != nullptr );
	spinCameraSpeed->SetValue( cameraMoveSensivity );
}

//-----------------------------------//

void CameraControls::onCameraTransform()
{
	// We need to switch to a neutral locale or else the text
	// conversion will lead to different results depending on
	// each machine due to different locale settings.
	LocaleSaveRestore c;
	
	TransformPtr transCamera = getCameraTransform();
	Vector3 position = transCamera->getPosition();
	
	char str[32];

	String::fromFloat( str, position.x );
	textX->ChangeValue( str );
	
	String::fromFloat( str, position.y );
	textY->ChangeValue( str );
	
	String::fromFloat( str, position.z );
	textZ->ChangeValue( str );
}

//-----------------------------------//

void CameraControls::updateCameraPosition()
{
	std::string str( textX->GetValue() );
	float X = String::toNumber<float>( str );
	
	str.assign( textY->GetValue() );
	float Y = String::toNumber<float>( str );
	
	str.assign( textZ->GetValue() );
	float Z = String::toNumber<float>( str );

	TransformPtr transCamera = getCameraTransform();
	transCamera->setPosition( Vector3(X, Y, Z) );
}

//-----------------------------------//

void CameraControls::onTextEnter( wxCommandEvent& )
{
	updateCameraPosition();
}

//-----------------------------------//

void CameraControls::buildControls()
{
	const short BAR_HEIGHT = 20;

	wxBoxSizer* sizer = new wxBoxSizer( wxHORIZONTAL );

	const wxString includes( "0123456789." );
	
	// X axis.
	wxStaticText* labelX;
	labelX = new wxStaticText( this, wxID_ANY, wxT("X:") );
	sizer->Add( labelX, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );
	
	wxTextValidator validatorX(wxFILTER_INCLUDE_CHAR_LIST, &X);
	validatorX.SetCharIncludes( includes );
	
	textX = new wxTextCtrl( this, wxID_ANY, wxT("0.0"),
		wxDefaultPosition, wxSize(50, BAR_HEIGHT),
		wxTE_CENTRE | wxTE_PROCESS_ENTER, validatorX);

	textX->Bind(wxEVT_COMMAND_TEXT_ENTER, &CameraControls::onTextEnter, this);

	sizer->Add( textX, 0, wxALIGN_CENTER_VERTICAL, 5 );

	// Y axis.
	wxStaticText* labelY;
	labelY = new wxStaticText( this, wxID_ANY, wxT("Y:") );
	sizer->Add( labelY, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );

	wxTextValidator validatorY(wxFILTER_INCLUDE_CHAR_LIST, &Y);
	validatorY.SetCharIncludes( includes );

	textY = new wxTextCtrl( this, wxID_ANY, wxT("0.0"),
		wxDefaultPosition, wxSize(50, BAR_HEIGHT),
		wxTE_CENTRE | wxTE_PROCESS_ENTER, validatorY);

	textY->Bind(wxEVT_COMMAND_TEXT_ENTER, &CameraControls::onTextEnter, this);
	sizer->Add( textY, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	// Z axis.
	wxStaticText* labelZ;
	labelZ = new wxStaticText( this, wxID_ANY, wxT("Z:") );
	sizer->Add( labelZ, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );
	
	wxTextValidator validatorZ(wxFILTER_INCLUDE_CHAR_LIST, &Z);
	validatorZ.SetCharIncludes( includes );
	
	textZ = new wxTextCtrl( this, wxID_ANY, wxT("0.0"),
		wxDefaultPosition, wxSize(50, BAR_HEIGHT),
		wxTE_CENTRE | wxTE_PROCESS_ENTER, validatorZ);

	textZ->Bind(wxEVT_COMMAND_TEXT_ENTER, &CameraControls::onTextEnter, this);
	sizer->Add( textZ, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	// Static line.
	wxStaticLine* staticLine;
	staticLine = new wxStaticLine( this, wxID_ANY, wxDefaultPosition,
		wxDefaultSize, wxLI_VERTICAL );
	sizer->Add( staticLine, 0, wxEXPAND|wxALL, 5 );

	wxStaticText* labelSpeed;
	labelSpeed = new wxStaticText( this, wxID_ANY, wxT("Speed:") );
	sizer->Add( labelSpeed, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5 );

	// Camera spin button.
	spinCameraSpeed = new wxSpinCtrlDouble( this, wxID_ANY,
		wxEmptyString, wxDefaultPosition, wxSize(60, BAR_HEIGHT) );
	
	spinCameraSpeed->Bind(wxEVT_COMMAND_SPINCTRLDOUBLE_UPDATED,
		&CameraControls::onCameraSpeedSpin, this);

	spinCameraSpeed->SetRange(0.0f, 1000.0f);
	spinCameraSpeed->SetIncrement(10.0f);
	
	sizer->Add( spinCameraSpeed, 0, wxALIGN_CENTER_VERTICAL, 5 );

	// Camera spin button.
	wxBitmap iconWireframe = wxMEMORY_BITMAP(grid_icon_small);
	buttonWireframe = new wxBitmapButton( this, wxID_ANY, iconWireframe,
		wxDefaultPosition, wxSize(-1, BAR_HEIGHT-1), wxBU_AUTODRAW );
	sizer->Add( buttonWireframe, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5 );
	
	wxBitmap iconTextures = wxMEMORY_BITMAP(map_small);
	buttonTextures = new wxBitmapButton( this, wxID_ANY, iconTextures,
		wxDefaultPosition, wxSize(-1, BAR_HEIGHT-1), wxBU_AUTODRAW );
	sizer->Add( buttonTextures, 0, wxALIGN_CENTER_VERTICAL, 5 );

	// View.
	wxString choiceViewChoices[] = 
		{ "Free", wxT("Top"), wxT("Down"), wxT("Left"), wxT("Right") };

	choiceView = new wxChoice( this, wxID_ANY, wxDefaultPosition,
		wxSize(-1, BAR_HEIGHT-1), VAPOR_ARRAY_SIZE(choiceViewChoices),
		choiceViewChoices, 0 );

	choiceView->SetSelection( 0 );
	sizer->Add( choiceView, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );

	SetSizerAndFit( sizer );
}

//-----------------------------------//

} } // end namespaces
