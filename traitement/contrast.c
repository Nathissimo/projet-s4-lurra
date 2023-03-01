#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "contrast.h"
#include <err.h>

int average(SDL_Surface* surface)
{
    int w = surface->w;
    int h = surface->h;

    int red = 0;
    int green = 0;
    int blue = 0;
	
    Uint32* pixels = surface -> pixels;
    for (int y = 0; y < w*h; y++)
    {
            Uint8 r, g, b;
            SDL_GetRGB(pixels[y], surface->format, &r, &g, &b);

            blue += b;
            green += g;
            red += r;
	
    }


    int pixelslen = w*h;
    return (red/pixelslen + green/pixelslen + blue/pixelslen) / 3;
}

Uint8 Truncate(double value)
{
	if (value < 0)
		return 0;
	else if (value> 255)
		return 255;
	else
		return (Uint8) value;
}



void delete_contrast ( SDL_Surface* surface, double C)
{
	Uint32* pixels = surface -> pixels;
	int len = surface->w * surface -> h;
	SDL_PixelFormat* format = surface -> format;
	C = average(surface);
	if (SDL_LockSurface(surface)<0)
         	errx(EXIT_FAILURE, "%s", SDL_GetError());
	
	double correction = (259*(C+255))/(255*(259-C));

	for (int i = 0; i<len; i++)
	{
		Uint8 r,g,b;
		SDL_GetRGB( pixels[i], format , &r, &g , &b);
		
		 r= Truncate( correction * (r-C)+C);
		 g= Truncate( correction * (g-C)+C);
		 b= Truncate( correction * (b-C)+C);
		 /*int min =0;
		 if (r< g && r<b)
			 min = r;
		 else if (g<b)
			 min =g;
		 else
			 min = b;
		*/
		 pixels[i] = SDL_MapRGB ( format, r, g,b);
	}
	SDL_UnlockSurface(surface);
}
