#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "fill.h"
#include <math.h>


void fill(Uint32* pixels, int x, int width, int height,int len, SDL_PixelFormat* format, Uint8 colorR, Uint8 colorG, Uint8 colorB)
{
	Uint8 actr, actg, actb;
	SDL_GetRGB(pixels[x], format, &actr, &actg, &actb);
	if (actr != 0 || actg != 0 || actb != 0)
		return;

	//printf("x : %i\n",x);

	pixels[x] = SDL_MapRGB(format, colorR,colorG,colorB);

	int placeX = x / width;
	int placeY = x % width;

	if (placeX-1 >= 0)
	{
		//printf("up x : %i\n",x);
		fill(pixels,(placeX+1)*width+placeY,width,height,len,format,colorR,colorG,colorB);
	}

	if (placeX+1 < height)
		fill(pixels,(placeX+1)*width+placeY,width,height,len,format,colorR,colorG,colorB);

	if (placeY-1 >= 0)
		fill(pixels,placeX*width+placeY-1,width,height,len,format,colorR,colorG,colorB);
	
	if (placeY+1 < width)
		fill(pixels,placeX*width+placeY+1,width,height,len,format,colorR,colorG,colorB);
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
		//printf("i : %i\n",i);
		Uint8 r, g, b;
		SDL_GetRGB(pixels[i], format, &r, &g, &b);
		//printf("r: %i g : %i b : %i\n",r,g,b);
		if (r == 0 && g == 0 && b == 0)
		{
			if ((i+20)% 256 == 0)
				fill(pixels, i, width, height, len, format, (i+10)% 256, 0, 0);
			else
				fill(pixels, i, width, height, len, format, (i+10)% 256, 0, 0);
		}


			/*
			if (i%50 == 0 && i%150 == 0 && i%256 == 0)
				fill(pixels, i, width, height, len, format, i%50, i%150, i%256);
			else
				fill(pixels, i, width, height, len, format, (i+1)%50, (i+1)%150, (i+1)%256);
			*/

	}
    SDL_UnlockSurface(surface); 
}
