/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Scene/Particles.h"
#include "Scene/Transform.h"
#include "Scene/Entity.h"
#include "Render/DebugGeometry.h"
#include "Render/GL.h"

namespace vapor {

//-----------------------------------//

REFLECT_ENUM(EmitterType)
	ENUM(Box)
	ENUM(Sphere)
	ENUM(Mesh)
REFLECT_ENUM_END()

REFLECT_CHILD_CLASS(Particles, Geometry)
	FIELD_ENUM(EmitterType, emitter)
	FIELD_PRIMITIVE(float, spawnRate)
	FIELD_PRIMITIVE(float, fadeRate)
	FIELD_PRIMITIVE(float, size)
	FIELD_PRIMITIVE(float, minLife)
	FIELD_PRIMITIVE(float, maxLife)
	FIELD_PRIMITIVE(Vector3, minVelocity)
	FIELD_PRIMITIVE(Vector3, maxVelocity)
	FIELD_PRIMITIVE(float, minScale)
	FIELD_PRIMITIVE(float, maxScale)
	FIELD_PRIMITIVE(Vector3, attenuation)
	FIELD_CLASS_PTR_SETTER(Image, ImageHandle, image, Handle, Image)
REFLECT_CLASS_END()

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
	, numParticles(0)
{ }

//-----------------------------------//

void Particles::setImage(const ImageHandle& image)
{
	Material* pMaterial = material.Resolve();
	pMaterial->setTexture(0, image);
}

//-----------------------------------//

void Particles::resetParticle(Particle& particle)
{
	particle.position.x = MathFloatRandom(0.0f, 1.0f);
	particle.position.y = MathFloatRandom(0.0f, 1.0f);
	particle.position.z = MathFloatRandom(0.0f, 1.0f);
		
	particle.velocity.x = MathFloatRandom(minVelocity.x, maxVelocity.x);
	particle.velocity.y = MathFloatRandom(minVelocity.y, maxVelocity.y);
	particle.velocity.z = MathFloatRandom(minVelocity.z, maxVelocity.z);

	particle.color = Color::White;
	particle.life = MathFloatRandom(minLife, maxLife);
	particle.alive = false;
}

//-----------------------------------//

void Particles::spawnParticles(int numSpawn)
{
	for(size_t i = 0; i < numParticles && numSpawn; i++)
	{
		Particle& particle = particles[i];

		if( particle.alive ) continue;

		resetParticle(particle);
		particle.alive = true;

		numSpawn--;
	}
}

//-----------------------------------//

void Particles::createGeometry()
{
	vb = Allocate(VertexBuffer, AllocatorGetHeap());
	
	material = MaterialCreate(AllocatorGetHeap(), "ParticlesMaterial");

	Material* pMaterial = material.Resolve();
	pMaterial->setDepthWrite(false);
	pMaterial->setBlending(BlendSource::SourceAlpha, BlendDestination::One);

	RenderablePtr renderable = Allocate(Renderable, AllocatorGetHeap(), PolygonType::Points);
	renderable->setVertexBuffer(vb);
	renderable->setMaterial(material);
	renderable->setRenderLayer(RenderLayer::Transparency);

	renderable->onPreRender.Bind( this, &Particles::onPreRender );
	renderable->onPostRender.Bind( this, &Particles::onPostRender ); 
	
	addRenderable(renderable);

	particles.resize(MAX_PARTICLES);
}

//-----------------------------------//

void Particles::update(float delta)
{
	if( renderables.empty() ) createGeometry();

	numParticles = particles.size();

	int numSpawn = ceil(spawnRate * delta);
	spawnParticles(numSpawn);

	std::vector<Vector3> positions;
	positions.reserve( numParticles );

	std::vector<Color> colors;
	colors.reserve( numParticles );

	// Update the particles.
	for(size_t i = 0; i < numParticles; i++)
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

void Particles::onPreRender(const RenderState&)
{
	glEnable(GL_POINT_SPRITE);
	
	glPointSize(size);
	glPointParameterfv( GL_POINT_DISTANCE_ATTENUATION, &attenuation.x );
	glPointParameterf( GL_POINT_SIZE_MIN, minScale );
	glPointParameterf( GL_POINT_SIZE_MAX, maxScale );
	glPointParameterf( GL_POINT_FADE_THRESHOLD_SIZE, fadeRate );
	//glTexEnvi( GL_POINT_SPRITE, GL_COORD_REPLACE, true );
}

//-----------------------------------//

void Particles::onPostRender(const RenderState&)
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