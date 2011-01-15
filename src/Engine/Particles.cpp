/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "scene/Particles.h"
#include "scene/Transform.h"
#include "scene/Entity.h"
#include "render/DebugGeometry.h"

namespace vapor {

//-----------------------------------//

BEGIN_ENUM(EmitterType)
	ENUM(Box)
	ENUM(Sphere)
	ENUM(Mesh)
END_ENUM()

BEGIN_CLASS_PARENT(Particles, Geometry)
	FIELD_ENUM(Particles, EmitterType, emitter)
	FIELD_PRIMITIVE(Particles, float, spawnRate)
	FIELD_PRIMITIVE(Particles, float, fadeRate)
	FIELD_PRIMITIVE(Particles, float, size)
	FIELD_PRIMITIVE(Particles, float, minScale)
	FIELD_PRIMITIVE(Particles, float, maxScale)
	FIELD_PRIMITIVE(Particles, Vector3, attenuation)
	FIELD_CLASS_PTR_SETTER(Particles, Image, image, Image)
END_CLASS()

const int MAX_PARTICLES = 512;

//-----------------------------------//

Particles::Particles()
	: emitter(EmitterType::Box)
	, size(10)
	, spawnRate(20)
	, fadeRate(60)
	, minScale(1)
	, maxScale(1)
	, attenuation(1, 0, 0)
	, image(nullptr)
	, numParticles(0)
{
	vb = new VertexBuffer();
	
	material = new Material("ParticlesMaterial");
	material->setDepthWrite(false);
	material->setBlending(BlendSource::SourceAlpha, BlendDestination::InverseSourceAlpha);

	RenderablePtr renderable = new Renderable(PolygonType::Points);
	renderable->setVertexBuffer(vb);
	renderable->setMaterial(material);
	
	renderable->onPreRender.Connect( this, &Particles::onPreRender );
	renderable->onPostRender.Connect( this, &Particles::onPostRender ); 
	
	addRenderable(renderable, RenderStage::Transparency);

	particles.resize(MAX_PARTICLES);
}

//-----------------------------------//

void Particles::setImage(const ImagePtr& image)
{
	this->image = image;
	material->setTexture(0, image);
}

//-----------------------------------//

void Particles::resetParticle(Particle& particle)
{
	particle.position.x = Math::random(0.0f, 1.0f);
	particle.position.y = Math::random(0.0f, 1.0f);
	particle.position.z = Math::random(0.0f, 1.0f);
		
	particle.velocity.x = Math::random(0.0f, 0.1f);
	particle.velocity.y = Math::random(0.0f, 0.1f);
	particle.velocity.z = Math::random(0.0f, 0.1f);

	particle.alive = true;
}

//-----------------------------------//

void Particles::spawnParticles(int numSpawn)
{
	for(uint i = 0; i < numParticles && numSpawn; i++)
	{
		Particle& particle = particles[i];

		if( particle.alive )
			continue;

		resetParticle(particle);
		numSpawn--;
	}
}

//-----------------------------------//

void Particles::update(double delta)
{
	numParticles = particles.size();

	int numSpawn = int(spawnRate / delta);
	spawnParticles(numSpawn);

	std::vector<Vector3> positions;
	positions.reserve( numParticles );

	// Update the particles.
	for(uint i = 0; i < numParticles; i++)
	{
		Particle& particle = particles[i];

		if( !particle.alive )
			continue;

		particle.position += particle.velocity;

		positions.push_back( particle.position );
	}

	vb->set(VertexAttribute::Position, positions);

	//updateDebugRenderable();
}

//-----------------------------------//

void Particles::onPreRender()
{
	glEnable(GL_POINT_SPRITE);
	
	glPointSize(size);
	glPointParameterfv( GL_POINT_DISTANCE_ATTENUATION, &attenuation.x );
	glPointParameterf( GL_POINT_SIZE_MIN, minScale );
	glPointParameterf( GL_POINT_SIZE_MAX, maxScale );
	glPointParameterf( GL_POINT_FADE_THRESHOLD_SIZE, fadeRate );
	glTexEnvi( GL_POINT_SPRITE, GL_COORD_REPLACE, true );
}

//-----------------------------------//

void Particles::onPostRender()
{
	glDisable(GL_POINT_SPRITE);
}

//-----------------------------------//

void Particles::updateDebugRenderable() const
{
	if( !debugRenderable )
		return;
}

//-----------------------------------//

RenderablePtr Particles::createDebugRenderable() const
{
	assert( !debugRenderable );
	return nullptr;
}

//-----------------------------------//

} // end namespace