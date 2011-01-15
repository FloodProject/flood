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
	FIELD_PRIMITIVE(Particles, float, minLife)
	FIELD_PRIMITIVE(Particles, float, maxLife)
	FIELD_PRIMITIVE(Particles, Vector3, minVelocity)
	FIELD_PRIMITIVE(Particles, Vector3, maxVelocity)
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
	, minLife(1)
	, maxLife(2)
	, minScale(1)
	, maxScale(20)
	, minVelocity(0, 0, 0)
	, maxVelocity(0, 1, 0)
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
		
	particle.velocity.x = Math::random(minVelocity.x, maxVelocity.x);
	particle.velocity.y = Math::random(minVelocity.y, maxVelocity.y);
	particle.velocity.z = Math::random(minVelocity.z, maxVelocity.z);

	particle.color = Color::White;
	particle.life = Math::random(minLife, maxLife);
	particle.alive = false;
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
		particle.alive = true;

		numSpawn--;
	}
}

//-----------------------------------//

void Particles::update(double delta)
{
	numParticles = particles.size();

	int numSpawn = ceil(spawnRate * delta);
	spawnParticles(numSpawn);

	std::vector<Vector3> positions;
	positions.reserve( numParticles );

	std::vector<Color> colors;
	colors.reserve( numParticles );

	// Update the particles.
	for(uint i = 0; i < numParticles; i++)
	{
		Particle& particle = particles[i];

		if( !particle.alive )
			continue;

		if( particle.life <= 0 )
		{
			particle.alive = false;
			continue;
		}

		particle.position += particle.velocity;
		particle.life -= float(delta);
		particle.color.a = particle.life / maxLife;

		positions.push_back( particle.position );
		colors.push_back( particle.color );
	}

	vb->set(VertexAttribute::Position, positions);
	vb->set(VertexAttribute::Color, colors);

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