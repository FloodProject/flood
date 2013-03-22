/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "RenderQueue.h"
#include "Material.h"
#include "Matrix4x3.h"
#include "Matrix4x4.h"
#include "RenderBatch.h"
#include "ResourceHandle.h"
#include "Texture.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::RenderState::RenderState(::RenderState* native)
{
    Renderable = gcnew Flood::RenderBatch((::RenderBatch*)native->renderable);
    Material = gcnew Flood::Material((::Material*)native->material);
    ModelMatrix = Flood::Matrix4x3((::Matrix4x3*)&native->modelMatrix);
    Priority = native->priority;
}

Flood::RenderState::RenderState(System::IntPtr native)
{
    auto __native = (::RenderState*)native.ToPointer();
    Renderable = gcnew Flood::RenderBatch((::RenderBatch*)__native->renderable);
    Material = gcnew Flood::Material((::Material*)__native->material);
    ModelMatrix = Flood::Matrix4x3((::Matrix4x3*)&__native->modelMatrix);
    Priority = __native->priority;
}

Flood::RenderState::RenderState(Flood::RenderBatch^ renderable)
{
}

Flood::LightState::LightState(::LightState* native)
{
    Depth = gcnew Flood::Texture((::Texture*)native->depth);
    Projection = Flood::Matrix4x4((::Matrix4x4*)&native->projection);
}

Flood::LightState::LightState(System::IntPtr native)
{
    auto __native = (::LightState*)native.ToPointer();
    Depth = gcnew Flood::Texture((::Texture*)__native->depth);
    Projection = Flood::Matrix4x4((::Matrix4x4*)&__native->projection);
}

Flood::RenderBlock::RenderBlock(::RenderBlock* native)
{
}

Flood::RenderBlock::RenderBlock(System::IntPtr native)
{
    auto __native = (::RenderBlock*)native.ToPointer();
}

