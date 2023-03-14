#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>

SDL_Surface* load_image(const char* path)

{
	SDL_Surface* surface_temp = IMG_Load(path);
	if (surface_temp == NULL)
		errx(EXIT_FAILURE, "%s", SDL_GetError());

	SDL_Surface* surface = SDL_ConvertSurfaceFormat( surface_temp, SDL_PIXELFORMAT_RGB888, 0);
	if (surface == NULL)
		errx(EXIT_FAILURE, "%s", SDL_GetError());

	SDL_FreeSurface(surface_temp);
	return surface;
}


size_t distance_pixels (Uint32 pixel1, Uint32 pixel2, format)
{
	Uint8 r1, g1, b1;
	SDL_GetRGB(pixel1, format, &r1, &g1, &b1);
	Uint8 r2, g2, b2;
	SDL_GetRGB(pixel2, format, &r2, &g2, &b2);

	float temp = powf( color1.r - color2.r, 2)+ pow( color1.g - color2.g, 2) + pow( color1.b - color2.b, 2);
	temp = sqrt(temp);
	return temp;

}

size_t def_nb_color(SDL_surface surface, size_t diff)
{
    Uint32* pixels = surface->pixels;

    SDL_PixelFormat* format = surface->format;

    if (diff > 255)
	err(3, "the difference beetween 2 color must be > 255");

    Uint32* pixels_took = malloc(sizeof(Uint32)* (int) ceil((255/diff));

    pixels_took[0] = pixels[0];
    int itook = 1;

    int len = surface->w * surface->h;

    for (int i = 1; i < len; i++)
    {
	if (to_add(pixels_took, pixels[i]) == 1)
	{
	    pixels_took[itook] = pixels[i];
	}

    }
}

int main(int argv, char** argc)
{
    if  (argv != 3)
	errx (1, "Usage : name of image and the min difference beetween 2 color\n");

    SDL_Surface* surface = load_image( argc[1]);

    printf("nb color : %lu", def_nb_color(surface, argc[2]));

    SDL_FreeSurface(surface);

    return 1;
}
