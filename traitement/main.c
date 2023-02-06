#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <assert.h>
#include "blur.h"
#include "binarization.h"
#include "dilatation_and_erosion.h"
#include "grayscale.h"
#include "sobel.h"
#include "rescale.h"


// Updates the display.
//
// renderer: Renderer to draw on.
// texture: Texture that contains the image.
void draw(SDL_Renderer* renderer, SDL_Texture* texture)
{
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

// Event loop that calls the relevant event handler.
//
// renderer: Renderer to draw on.
// colored: Texture that contains the colored image.
// grayscale: Texture that contains the grayscale image.
void event_loop(SDL_Renderer* renderer, SDL_Texture* colored, SDL_Texture* last)
{
    SDL_Texture* t = colored;

    draw(renderer, t);

    SDL_Event event;

    while (1)
    {
        SDL_WaitEvent(&event);

        switch (event.type)
        {
            case SDL_QUIT:
		return;
	
	    case SDL_WINDOWEVENT:
		if (event.window.event == SDL_WINDOWEVENT_RESIZED)
			draw(renderer, t);
		break;

	    case SDL_KEYDOWN:
		if (t == colored)
			t = last;
		else
			t = last;
		draw(renderer, t);
		break;
        }
    }
}

// Loads an image in a surface.
// The format of the surface is SDL_PIXELFORMAT_RGB888.
//
// path: Path of the image.
SDL_Surface* load_image(const char* path)
{
    SDL_Surface* surface = IMG_Load(path);

    SDL_Surface* surface_rvb = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGB888,0);
    if (surface_rvb == NULL)
	    errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    SDL_FreeSurface(surface);

    return surface_rvb;
}



int main(int argc, char** argv)
{
	// Initial width and height of the window.
	const int INIT_WIDTH = 640;
	const int INIT_HEIGHT = 400;

    // Checks the number of arguments.
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");

    // - Initialize the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Create a window.
    SDL_Window* window = SDL_CreateWindow("Traitement", 0, 0, INIT_WIDTH, INIT_HEIGHT,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Create a renderer.
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Create a surface from the colored image.
    SDL_Surface* temp_surface = load_image(argv[1]);
    if (temp_surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    size_t neww;
    size_t newh;
    size_t min_size = 1000;
    if (temp_surface->w > temp_surface->h) {
        newh = min_size;
        neww = min_size*temp_surface->w/temp_surface->h;
    }
    else {
        neww = min_size;
        newh = min_size*temp_surface->h/temp_surface->w;
    }
    //printf("w%ld  h%ld\n", neww, newh);
    SDL_Surface *surface = resize(temp_surface, neww, newh);
    if (surface == NULL)
    {
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    }

	SDL_SaveBMP(surface, "temp_files/colored.png");

	// - Create a texture from the colored surface.
    
    SDL_Texture* texture_colored = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture_colored == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
        
    // - Convert the surface into grayscale.
    
    surface_to_grayscale(surface);

    SDL_SaveBMP(surface, "temp_files/grayscale.png");
     
    // - Convert the surface into blur.
    
    for (int i = 0; i < 5; i++)
        surface_to_blur(surface);
    
    SDL_SaveBMP(surface, "temp_files/blur.png");

       
    // - Convert the surface into sobel.
    
    surface_to_sobel(surface);
    
    SDL_SaveBMP(surface, "temp_files/sobel.png");

    // - Convert the surface into binarization.
    
    surface_to_binarization(surface);

    SDL_SaveBMP(surface, "temp_files/binarization.png");
    
    // - Create a texture from the colored surface.
    
    SDL_Texture* texture_last = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture_last == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Free the surface.
    
    SDL_FreeSurface(surface);

    // - Dispatch the events.
    
    draw(renderer, texture_colored); //draw here because draw in event_loop don't make it right
    event_loop(renderer, texture_colored, texture_last);

    // - Destroy the objects.

    SDL_DestroyTexture(texture_last);
    SDL_DestroyTexture(texture_colored);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
