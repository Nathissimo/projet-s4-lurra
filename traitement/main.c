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
void event_loop(SDL_Renderer* renderer, SDL_Texture* colored, SDL_Texture* grayscale)
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
			t = grayscale;
		else
			t = colored;
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
    SDL_Window* window = SDL_CreateWindow("grayscale", 0, 0, INIT_WIDTH, INIT_HEIGHT,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Create a renderer.
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Create a surface from the colored image.
    
    SDL_Surface* surface = load_image(argv[1]);
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Resize the window according to the size of the image.
     
    SDL_SetWindowSize(window, surface->w, surface->h);
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Create a texture from the colored surface.
    
    SDL_Texture* texture_colored = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture_colored == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    // - Convert the surface into grayscale.
    
    surface_to_grayscale(surface);

    // - Create a new texture from the grayscale surface.
    
    SDL_Texture* texture_grayscale = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture_grayscale == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Free the surface.
    
    SDL_FreeSurface(surface);

    // - Dispatch the events.
    
    draw(renderer, texture_colored); //draw here because draw in event_loop don't make it right
    event_loop(renderer, texture_colored, texture_grayscale);

    // - Destroy the objects.

    SDL_DestroyTexture(texture_grayscale);
    SDL_DestroyTexture(texture_colored);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
