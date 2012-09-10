/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2012 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#ifndef _SDL_config_flood_h
#define _SDL_config_flood_h

#include "SDL_platform.h"

/**
 *  \file SDL_config_flood.h
 *  
 *  This is the configuration that is used to build the SDL with target
 *  features intended for use in the Flood game engine runtime. This is
 *  based on SDL_config_minimal.h
 */

#include <stddef.h>
#include <stdarg.h>

#if !defined(_STDINT_H_) && !defined(_STDINT_H) && (!defined(HAVE_STDINT_H) || !_HAVE_STDINT_H)
typedef unsigned int size_t;
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed short int16_t;
typedef unsigned short uint16_t;
typedef signed int int32_t;
typedef unsigned int uint32_t;
typedef signed long long int64_t;
typedef unsigned long long uint64_t;
typedef unsigned long uintptr_t;
#endif /* if (stdint.h isn't available) */

#ifdef __GNUC__
#define HAVE_GCC_SYNC_LOCK_TEST_AND_SET 1
#endif

/* Enable the dummy audio driver (src/audio/dummy/\*.c) */
#define SDL_AUDIO_DRIVER_DUMMY	1

/* Enable the stub joystick driver (src/joystick/dummy/\*.c) */
#define SDL_JOYSTICK_DISABLED	1

/* Enable the stub haptic driver (src/haptic/dummy/\*.c) */
#define SDL_HAPTIC_DISABLED	1

/* Enable the stub shared object loader (src/loadso/dummy/\*.c) */
#define SDL_LOADSO_DISABLED	1

/* Enable the stub thread support (src/thread/generic/\*.c) */
#define SDL_THREADS_DISABLED	1

/* Enable the stub timer support (src/timer/dummy/\*.c) */
#define SDL_TIMERS_DISABLED	1

/* Enable dummy / Windows video drivers, and OpenGL (src/video/dummy/\*.c) */
#define SDL_VIDEO_DRIVER_DUMMY	1

#ifdef __WIN32__
#define SDL_VIDEO_DRIVER_WINDOWS 1
#endif

#ifndef SDL_VIDEO_OPENGL
#define SDL_VIDEO_OPENGL	1
#endif
#ifndef SDL_VIDEO_OPENGL_WGL
#define SDL_VIDEO_OPENGL_WGL	1
#endif
#ifndef SDL_VIDEO_RENDER_OGL
#define SDL_VIDEO_RENDER_OGL	1
#endif

/* Disable platform specific power support */
#define SDL_POWER_DISABLED 1

#endif /* _SDL_config_flood_h */
