/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

%ignore ResourcesInitialize;
%ignore ResourcesDeinitialize;

%import "Resources/API.h"

%ignore ResourceTaskRun;

%include "Resources/Resource.h"

%extend ResourceManager
{
	static Resource* Load(const Path& name)
	{
		return GetResourceManager()->loadResource(name).Resolve();
	}
}

%include "Resources/ResourceManager.h"

%include "Resources/Image.h"
%include "Resources/Material.h"

%ignore SoundStream;
%ignore SOUND_LOAD_STREAM;
//%include "Resources/Sound.h"

//%include "Resources/Mesh.h"
//%include "Resources/Skeleton.h"
//%include "Resources/Bone.h"
//%include "Resources/Animation.h"
//%include "Resources/Attachment.h"