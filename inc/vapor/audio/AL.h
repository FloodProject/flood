/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef VAPOR_AUDIO_OPENAL

#ifdef VAPOR_PLATFORM_MACOSX
	#include <OpenAL/al.h>
	#include <OpenAL/alc.h>
#else
	#include <al.h>
	#include <alc.h>
#endif

#endif