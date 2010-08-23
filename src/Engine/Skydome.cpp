/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/Engine.h"
#include "vapor/scene/Skydome.h"
#include "vapor/scene/Camera.h"
#include "vapor/scene/Node.h"
#include "vapor/math/Math.h"
#include "vapor/math/Noise.h"

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT(Skydome, Geometry)
	FIELD_PRIMITIVE(Skydome, float, currentTime)
	FIELD_PRIMITIVE(Skydome, Color, skyColorTop)
	FIELD_PRIMITIVE(Skydome, Color, skyColorBottom)
	FIELD_PRIMITIVE(Skydome, bool, showClouds)
	FIELD_PRIMITIVE(Skydome, bool, followCamera)
	FIELD_CLASS_PTR(Skydome, Image, imgClouds)
END_CLASS()

//-----------------------------------//

Skydome::Skydome()
	: currentTime(0)
	, followCamera(true)
	, skyColorTop(Color::Black)
	, skyColorBottom(Color::SkyBlue)
{
	Class& klass = getType();
	klass.onFieldChanged += fd::bind(&Skydome::onFieldChanged, this);

	generateDome();
	generateBodies();
}

//-----------------------------------//

Skydome::~Skydome()
{
	Class& klass = getType();
	klass.onFieldChanged -= fd::bind(&Skydome::onFieldChanged, this);
}

//-----------------------------------//

void Skydome::onFieldChanged(const Field& field)
{
	setSkyLinearGradient(skyColorTop, skyColorBottom);
}

//-----------------------------------//

void Skydome::setClouds( const ImagePtr& clouds )
{
	imgClouds = clouds;
}

//-----------------------------------//

const float DEFAULT_SPHERE_RADIUS = 10000.0f;

void Skydome::generateDome()
{
	MaterialPtr mat( new Material("Sky") );
	//mat->setDepthWrite( false );
	mat->setProgram( "Sky" );
	mat->setBlending( BlendingSource::One, BlendingDestination::InverseSourceAlpha );

	dome = new Sphere( false, 2, DEFAULT_SPHERE_RADIUS );
	dome->setPolygonMode( PolygonMode::Solid );
	dome->setMaterial( mat );
	
	setSkyLinearGradient(skyColorTop, skyColorBottom);

	addRenderable( dome );
}

//-----------------------------------//

void Skydome::generateBodies()
{
	// Clouds.
	Noise noise;

	std::vector<float> fn;
	noise.generate(fn, 256, 256);

	foreach( float& n, fn )
		n = pow((n+1) / 2.0f, 3.0f);
	
	std::vector<byte> bn;

	foreach( float& n, fn )
		bn.push_back( n*255 );

	ImagePtr image = new Image(256, 256, PixelFormat::Depth);
	image->setStatus( ResourceStatus::Loaded );
	image->setBuffer(bn);

	//TexturePtr tex = new Texture(image);

	//sun = new Quad( 100.0f, 100.0f );
	//MaterialPtr matSun( new Material( "SunBlend" ) );
	//matSun->setProgram( "tex" );
	//matSun->setTexture( 0, "sun.png" );
	//matSun->setBlending( BlendingSource::SourceAlpha,
	//	BlendingDestination::OneMinusSourceAlpha );
	//matSun->setBackfaceCulling( false );
	//sun->setMaterial( matSun );
	//addRenderable( sun );

	//MaterialPtr mat( new Material( "MoonBlend" ) );
	//mat->setProgram( "tex" );
	//mat->setTexture( 0, "moon.png" );
	//mat->setBlending( BlendingSource::SourceAlpha, BlendingDestination::OneMinusSourceAlpha );
	//mat->setBackfaceCulling( false );

	//RenderablePtr moon( new Quad(25.0f, 25.0f) );
	//moon->setMaterial( mat );
	//addRenderable( moon );
}

//-----------------------------------//

CameraPtr getCurrentCamera()
{
	Engine* engine = Engine::getInstancePtr();
	RenderDevice* device = engine->getRenderDevice();
	RenderTarget* target = device->getRenderTarget();
	const ViewportList& viewports = target->getViewports();
	View* view = viewports.front();
	return view->getCamera();
}

//-----------------------------------//

void Skydome::update( double delta )
{
	if( followCamera )
	{
		CameraPtr camera = getCurrentCamera();

		// Gets the current camera position.
		TransformPtr transCamera = camera->getNode()->getTransform();
		Vector3 cameraPosition = transCamera->getPosition();

		// Sets the sky position to the camera position.
		NodePtr nodeSky = getNode();
		TransformPtr transSky = nodeSky->getTransform();
		transSky->setPosition( cameraPosition );
	}

	// Updates the current game time.
	#pragma TODO("Create a game time subsystem")

	currentTime += delta;

	if(dome)
	{
		MaterialPtr material = dome->getMaterial();
		ProgramPtr program = material->getProgram();

		if(program)
		{
			program->bind();
			program->setUniform("vp_TexScroll", float(currentTime / 100.0f));
		}

		if( showClouds )
			material->setTexture(0, imgClouds);
		else
			material->setTexture(0, "alpha_null.png");
	}
}

//-----------------------------------//

Vector3 Skydome::getSunPosition()
{
	// We'll use spherical coordinates to get the new position for the
	// sun. These are pretty simple to use and adapt pretty well to 
	// what we want to achieve here.

	float rho = DEFAULT_SPHERE_RADIUS*0.95f;
	float phi = Math::PI*currentTime*0.01f;
	float theta = 0;

	// We want 12 hours of virtual time to correspond to the day cycle,
	// so the sun should start at half-sphere when time reaches a given
	// sunrise time (say 6am), and go on until the sunset time.

	float x = rho*sin(phi)*cos(theta);
	float y = rho*sin(phi)*sin(theta);
	float z = rho*cos(phi);

	return Vector3( x, y, z );
}

//-----------------------------------//

void Skydome::setSunNode( const NodePtr& sunNode )
{
	sun = sunNode->getTransform();
}

//-----------------------------------//

void Skydome::setSkyColor( const Color& color )
{
	VertexBufferPtr vb = dome->getVertexBuffer();
	uint numVertices = vb->getNumVertices();

	std::vector<Vector3> colors;
	colors.resize( numVertices, Vector3( color.r, color.g, color.b ) );

	vb->set( VertexAttribute::Color, colors );
}

//-----------------------------------//

float Skydome::scale( float number )
{
	const float rMin = 0.0f;
	const float rMax = 1.0f;

	number += abs(yMin);
	return (number / ((yMax) / (rMax - rMin))) + rMin;
}

//-----------------------------------//

void Skydome::setSkyLinearGradient( const Color& c1, const Color& c2 )
{
	VertexBufferPtr vb = dome->getVertexBuffer();
	const std::vector<Vector3>& vertices = vb->getVertices();

	// Search for the max and min Y coordinate.
	yMin = Limits::FloatMaximum;
	yMax = Limits::FloatMinimum;

	foreach( const Vector3& vec, vertices )
	{
		if( vec.y > yMax )
			yMax = vec.y;
		else if( vec.y < yMin )
			yMin = vec.y;
	}

	yMax += abs(yMin);
	//yMin += abs(yMin);

	std::vector<Vector3> colors;

	foreach( const Vector3& vec, vertices )
	{
		const Color& color = getSkyVertexColor( vec );
		colors.push_back( color );
	}	

	vb->set( VertexAttribute::Color, colors );
}

//-----------------------------------//

Color Skydome::getSkyVertexColor( const Vector3& v )
{
	float s = scale( v.y );
	float t = (s <= 0.5f) ? 0.0f : s - 0.5f;

	// Linear interpolate to get the color at the vertex.
	Vector3 newColor = ((Vector3) skyColorBottom).lerp(skyColorTop, t);

	return Color( newColor.x, newColor.y, newColor.z );
}

//-----------------------------------//

} // end namespace
