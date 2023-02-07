#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "fill.h"
#include <math.h>


void fill(Uint32* pixels, int x, int width, int height,long len, SDL_PixelFormat* format, Uint8 colorR, Uint8 colorG, Uint8 colorB)
{
	pixels[x] = SDL_MapRGB(format, colorR,colorG,colorB);

	int placeX = x / width;
	int placeY = x % width;

	long next_pos;

	Uint8 actr, actg, actb;

	for (int i = - NEIGHBOURD; i <= NEIGHBOURD; i++)
	{
		for (int j = - NEIGHBOURD; j <= NEIGHBOURD; j++)
		{
			next_pos = ((placeX+i)*width)+placeY+j;
			if (x > 10000)
				printf("place : %li len : % li\n",next_pos, len);
			if (next_pos >= 0 && next_pos < len && i != j)
			{
				SDL_GetRGB(
                    pixels[next_pos], format, &actr, &actg, &actb);
				if (actr == 0 && actg == 0 && actb == 0)
					fill(pixels,next_pos,width,height,len,format, colorR, colorG, colorB);
			}
		}
	}
}



void surface_to_fill(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;

    long len = surface->w * surface->h;

    int width = surface->w;
    
    int height = surface->h;
	
    SDL_PixelFormat* format = surface->format;

    SDL_LockSurface(surface);

    for (int i = 0; i < len; i++)
    {
		Uint8 r, g, b;
		SDL_GetRGB(pixels[i], format, &r, &g, &b);
		if (r == 0 && g == 0 && b == 0)
		{
			if (i*10 == 0 && i+10 == 0 && i*5 == 0)
				fill(pixels, i, width, height, len, format, i*10, i+10, i*5);
			else
				fill(pixels, i, width, height, len, format, i+1, i+10, i);
	    }
	}
    SDL_UnlockSurface(surface); 
}
