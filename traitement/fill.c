#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "fill.h"
#include <math.h>


void fill(Uint32* pixels, int x, int width, int height, SDL_PixelFormat* format, Uint8 colorR, Uint8 colorG, Uint8 colorB)
{
	pixels[x] = SDL_MapRGB(format, colorR,colorG,colorB);
	if (x % 100 == 0)
		printf("x = %i\n", x);
	int placeX = x / width;
	int placeY = x % width;
	
	Uint8 actr, actg, actb;

	for (int i = - NEIGHBOURD; i <= NEIGHBOURD; i++)
	{
		for (int j = - NEIGHBOURD; j <= NEIGHBOURD; j++)
		{
			if (placeX + i >= 0 && placeX + i < height &&
				placeY + j >= 0 && placeY + j < width &&
				i != j)
			{
				SDL_GetRGB(
                    pixels[((placeX+i)*width)+placeY+j], format, &actr, &actg, &actb);
				if (actr == 255 && actg == 255 && actb == 255)
					fill(pixels,((placeX+i)*width)+placeY+j,width,height,format, colorR, colorB, colorG);
			}
		}
	}
}



void surface_to_fill(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;

    int len = surface->w * surface->h;

    int width = surface->w;
    
    int height = surface->h;
	
    SDL_PixelFormat* format = surface->format;

    SDL_LockSurface(surface);

    for (int i = 0; i < len; i++)
    {
		Uint8 r, g, b;
		SDL_GetRGB(pixels[i], format, &r, &g, &b);
		if (r == 255 && g == 255 && b == 255)
	    	fill(pixels, i, width, height, format, i*10, i+10, i*5);
	}
    SDL_UnlockSurface(surface); 
}
