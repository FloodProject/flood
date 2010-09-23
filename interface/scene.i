/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

//%shared_ptr(vapor::Component)
//%shared_ptr(vapor::Transform)
//%shared_ptr(vapor::Geometry)
//%shared_ptr(vapor::Light)

%include "vapor/scene/Component.h"
%include "vapor/scene/Transform.h"
%include "vapor/scene/Model.h"
%include "vapor/scene/Camera.h"
%include "vapor/scene/Geometry.h"
%include "vapor/scene/Light.h"

%warnfilter(401) Node;
%include "vapor/scene/Node.h"
%include "vapor/scene/Group.h"
%include "vapor/scene/Scene.h"
