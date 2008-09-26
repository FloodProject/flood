/* sdl_picofont

   http://nurd.se/~noname/sdl_picofont

   File authors:
      Fredrik Hultin

   License: GPLv2
*/

#ifndef SDL_PICOFONT_H
#define SDL_PICOFONT_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <SDL.h>

SDL_Surface* FNT_Render(const char* text, SDL_Color color);
SDL_Surface* FNT_RenderMax(const char* text, unsigned int len, SDL_Color color);

unsigned char* FNT_GetFont();


#ifdef __cplusplus
};
#endif /* __cplusplus */

#endif /* SDL_PICOFONT_H */
