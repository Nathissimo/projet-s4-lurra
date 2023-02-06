#ifndef NEIGHBOURD

#define NEIGHBOURD 1

#endif
#ifndef FILL_H
#define FILL_H

void fill(Uint32* pixels, int x, int width, int height, SDL_PixelFormat* format, Uint8 colorR, Uint8 colorG, Uint8 colorB);

void surface_to_fill(SDL_Surface* surface);

#endif
