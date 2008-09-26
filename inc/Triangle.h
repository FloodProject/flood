#pragma once

#include "Renderable.h"
#include "Vector3.h"

namespace fuck {
namespace renderer {

class Triangle : Renderable
{
	math::Vertex vertices[3];
};

}
}
