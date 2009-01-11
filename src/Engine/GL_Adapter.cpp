/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/render/opengl/GL_Device.h"
#include "vapor/render/opengl/GL_Adapter.h"

#ifdef VAPOR_RENDERER_OPENGL

namespace vapor {
	namespace render {
		namespace opengl {

GLAdapter::GLAdapter(void)
{
	_cardName = (const char*) glGetString(GL_RENDERER);
	_cardVendor = (const char*) glGetString(GL_VENDOR);
	_driverVersion = (const char*) glGetString(GL_VERSION);

	info("render::opengl::adapter", "%s\nSupports OpenGL %s", _cardName, _driverVersion);
}

} } } // end namespaces

#endif