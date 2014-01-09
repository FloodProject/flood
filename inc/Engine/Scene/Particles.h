/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Engine/Scene/Geometry.h"
#include "Core/Math/Vector.h"
#include "Core/Math/Color.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

struct API_SCENE Particle
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

struct API_SCENE EmitterType
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

API_SCENE REFLECT_DECLARE_CLASS(Particles)

class API_SCENE Particles : public Geometry
{
	REFLECT_DECLARE_OBJECT(Particles)

public:

	Particles();

	// Updates this component.
	void update( float delta );

	// Sets the image of the particles.
	void setImage(const ImageHandle& image);

	// Updates the debug.renderable.
	void updateDebugRenderable() const;

	// Creates the debug.renderable.
    RenderBatchPtr createDebuRenderable() const;

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
	Vector<Particle> particles;

	// Number of active particles.
	uint numParticles;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Particles );

//-----------------------------------//

NAMESPACE_ENGINE_END