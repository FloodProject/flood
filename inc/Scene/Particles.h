/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Scene/Geometry.h"
#include "Math/Vector.h"
#include "Math/Color.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

struct Particle
{
	Particle() 
		: alive(false)
		, life(0)
	{}

	Vector3 position;
	Vector3 velocity;
	Color color;

	float life;
	bool alive;
};

//-----------------------------------//

/**
 * Particles can have different emitters.
 */

struct EmitterType
{
	REFLECT_DECLARE_ENUM(EmitterType)

	enum Enum
	{
		Box,
		Sphere,
		Mesh
	};
};

//-----------------------------------//

REFLECT_DECLARE_CLASS(Particles)

class API_ENGINE Particles : public Geometry
{
	REFLECT_DECLARE_OBJECT(Particles)

public:

	Particles();

	// Updates this component.
	void update( float delta );

	// Sets the image of the particles.
	void setImage(const ImageHandle& image);

	// Updates the debug renderable.
	void updateDebugRenderable() const;

	// Creates the debug renderable.
	RenderablePtr createDebugRenderable() const;

protected:

	// Creates the geometry.
	void createGeometry();

	// Spawns new particles.
	void spawnParticles(int numSpawn);

	// Resets a particle.
	void resetParticle(Particle& particle);

	// Pre-render callback.
	void onPreRender(RenderView*, const RenderState&);

	// Post-render callback.
	void onPostRender(RenderView*, const RenderState&);

	// Emitter type.
	EmitterType::Enum emitter;

	// Spawn rate.
	float spawnRate;

	// Fade rate.
	float fadeRate;

	// Point size.
	float size;

	// Mininum life.
	float minLife;

	// Maximum life.
	float maxLife;

	// Mininum scale size.
	float minScale;

	// Maximum scale size.
	float maxScale;

	// Mininum velocity.
	Vector3 minVelocity;

	// Maximum velocity.
	Vector3 maxVelocity;

	// Distance attenuation.
	Vector3 attenuation;

	// Vertex buffer.
	GeometryBufferPtr gb;

	// Material.
	MaterialHandle material;

	// Image.
	ImageHandle image;

	// Particles.
	std::vector<Particle> particles;

	// Number of active particles.
	uint numParticles;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Particles );

//-----------------------------------//

NAMESPACE_ENGINE_END