#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "fill.h"
#include <math.h>


void fill(Uint32* pixels, size_t x, size_t width, size_t height,size_t len, SDL_PixelFormat* format, Uint8 colorR, Uint8 colorG, Uint8 colorB)
{
	pixels[x] = SDL_MapRGB(format, colorR,colorG,colorB);

	size_t placeX = x / width;
	size_t placeY = x % width;

	size_t next_pos;

	Uint8 actr, actg, actb;

	for (int i = - NEIGHBOURD; i <= NEIGHBOURD; i++)
	{
		for (int j = - NEIGHBOURD; j <= NEIGHBOURD; j++)
		{
			if (next_pos < len && i != j)
			{
				SDL_GetRGB(pixels[next_pos], format, &actr, &actg, &actb);
				if (actr == 0 && actg == 0 && actb == 0)
				{
				    fill(pixels,next_pos,width,height,len,format, colorR, colorG, colorB);
				}
			}
		}
	}
}



void surface_to_fill(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;

    size_t len = surface->w * surface->h;

    size_t width = surface->w;
    
    size_t height = surface->h;

    SDL_PixelFormat* format = surface->format;

    SDL_LockSurface(surface);

    for (size_t i = 0; i < len; i++)
    {
		Uint8 r, g, b;
		SDL_GetRGB(pixels[i], format, &r, &g, &b);
		if (r == 0 && g == 0 && b == 0)
		{
			if (i*10 == 0 && i+10 == 0 && i*5 == 0)
				fill(pixels, i, width, height, len, format, i*10, i+10, i*5);
			else
				fill(pixels, i, width, height, len, format, i+5, i+10, 0);
	    }
	}
    SDL_UnlockSurface(surface); 
}
