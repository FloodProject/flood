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

%include "Scene/Component.h"
%include "Scene/Transform.h"
%include "Scene/Model.h"
%include "Scene/Camera.h"
%include "Scene/Geometry.h"
%include "Scene/Light.h"
%include "controllers/Controller.h"
%include "physics/CharacterController.h"

%warnfilter(401) Entity;
%include "Scene/Entity.h"
%include "Scene/Group.h"
%include "Scene/Scene.h"
