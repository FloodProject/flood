/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "scene/Geometry.h"
#include "math/Vector3.h"
#include "math/Color.h"

namespace vapor {

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
	DECLARE_ENUM()

	enum Enum
	{
		Box,
		Sphere,
		Mesh
	};
};

//-----------------------------------//

class VAPOR_API Particles : public Geometry
{
	DECLARE_CLASS_()

public:

	Particles();

	// Updates this component.
	void update( float delta );

	// Sets the image of the particles.
	void setImage(const ImagePtr& image);

	// Updates the debug renderable.
	void updateDebugRenderable() const;

	// Creates the debug renderable.
	RenderablePtr createDebugRenderable() const;

protected:

	// Spawns new particles.
	void spawnParticles(int numSpawn);

	// Resets a particle.
	void resetParticle(Particle& particle);

	// Pre-render callback.
	void onPreRender();

	// Post-render callback.
	void onPostRender();

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
	VertexBufferPtr vb;

	// Material.
	MaterialPtr material;

	// Image.
	ImagePtr image;

	// Particles.
	std::vector<Particle> particles;

	// Number of active particles.
	uint numParticles;
};

TYPEDEF_SHARED_POINTER_FROM_TYPE( Particles );

//-----------------------------------//

} // end namespace