/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Graphics/RenderCommand.h"
#include "Graphics/RenderState.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

/**
 * Use these different kinds of render groups to signal to the renderer
 * how you want your RenderBatchs to be sorted by the render device.
 * Lower numbers render before higher numbers.
 */

enum struct RenderLayer : uint8
{
	Normal,
	Transparency,
	PostTransparency,
	Overlays
};

//-----------------------------------//

union API_GRAPHICS RenderSortingKey
{
	uint64 key;
	
	struct
	{
		RenderLayer layer : 3;
		uint8 priority : 4;
		bool translucent : 1;
		uint32 depth : 24;
		uint32 material : 8;
	};
};

static_assert(sizeof(RenderSortingKey) == 8,
			  "Sorting key needs to be 64-bit");

/**
 * Represents a batch operation in the render proccess and contains the
 * data the render device needs to issue the draw calls for the object.
 */

struct API_GRAPHICS RenderBatch
{
	RenderBatch();

	DrawCallCommand drawCommand;
	RenderSortingKey sortingKey;

	uint8 numStateGroups;
	RenderStateGroup stateGroups[1];
};

typedef RenderBatch Renderable;

API_GRAPHICS RenderBatch* RenderBatchCreate(Allocator*);

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( RenderBatch );
typedef std::vector<RenderBatchPtr> RenderBatchsVector;

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Renderable );
typedef std::vector<RenderablePtr> RenderablesVector;

//-----------------------------------//

NAMESPACE_GRAPHICS_END