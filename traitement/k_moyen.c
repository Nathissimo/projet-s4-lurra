#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <sys/random.h>
#include <time.h>

SDL_Surface* RedimensionnerImage(SDL_Surface* surface, int largeur, int hauteur)
{
    SDL_Surface* surfaceRedimensionnee = NULL;

    surfaceRedimensionnee = SDL_CreateRGBSurface(0, largeur, hauteur, 32, 0, 0, 0, 0);

    SDL_SoftStretch(surface, NULL, surfaceRedimensionnee, NULL);
    SDL_FreeSurface(surface);
    return surfaceRedimensionnee;
}


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

void Random ( int* pix, SDL_Surface* surface, int k)
{
    int Row = surface->h;
    int Col = surface->w;

    srandom (time ( NULL));
    for (int i =0; i< k ; i++)
    {
	int temp1 = 0;

	if (getrandom (&temp1, sizeof (int), GRND_NONBLOCK) == -1)
	    perror ("getramdom");

	if ( temp1 < 0 )
	    temp1 = temp1 *-1;


	pix[i] = temp1 % (Row * Col);
    }
}

float distance_color (SDL_Color color1, SDL_Color color2)
{
    float temp = powf( color1.r - color2.r, 2)+ pow( color1.g - color2.g, 2) + pow( color1.b - color2.b, 2);
    temp = sqrt(temp);
    return temp;

}

SDL_Color Choose_Color (SDL_Surface* surface, SDL_Color* all_color, int i, int k )
{
    SDL_Color original_color;
    Uint32* pixels = surface->pixels;

    SDL_GetRGB ( pixels[i], surface->format , &original_color.r, &original_color.g, &original_color.b );


    //compare each color

    int index = 0;
    float min_distance = distance_color( original_color, all_color[0]);
    for ( int j =1 ; j< k ; j ++)
    {
	float temp = distance_color( original_color, all_color[j]);

	if ( min_distance > temp)   
	{
	    index = j;
	    min_distance = temp;
	}


    }
    return all_color[index];
}





void create_k_color (SDL_Color* all_color, SDL_Surface* original_surface, SDL_Surface* new_surface, int k)
{
    unsigned long* moyen = calloc ( k*3 ,sizeof (unsigned long) );
    unsigned long* nb_pixels = calloc (k, sizeof (unsigned long));

    Uint32* pixels_orinal = original_surface->pixels;
    Uint32* pixels_new = new_surface->pixels;
    SDL_PixelFormat* format_original = original_surface->format;
    SDL_PixelFormat* format_new = new_surface->format;

    int len = original_surface->w * original_surface->h;

    if ( len != new_surface->h * new_surface->w)
	errx (1, "the copy of the image is not the same size");


    //fill in the two arrays: the number of pixels to color 
    //and the addition of all the pixels according to their color
    for (int i = 0; i < len; i++ )
    {

	SDL_Color color_new;
	SDL_GetRGB ( pixels_new[i] , format_new, &color_new.r, &color_new.g, &color_new.b);
	for ( int j = 0; j< k ; j++)
	{
	    SDL_Color temp = all_color[j];
	    if ( temp.r == color_new.r && temp.g == color_new.g && temp.b == color_new.b)
	    {
		SDL_Color color_pix;
		SDL_GetRGB ( pixels_orinal[i] , format_original, &color_pix.r, &color_pix.g, &color_pix.b);

		//add the color in array	
		moyen[j*3] += color_pix.r;
		moyen[j*3+1] += color_pix.g;
		moyen[j*3+2] += color_pix.b;

		nb_pixels[j] ++;				
		break;
	    }
	}

    }
    // verifie si tt les pixels sont pris en compte
    // on doit le supprimer

    int nb = 0;
    for ( int i = 0 ; i < k ; i++ )
    {
	nb += nb_pixels[i];
	/*		printf ("nb_pixels %li\n", nb_pixels[i]);
			printf ("moyen[%i] = %li \n", i*3, moyen[i*3]);
			printf ("moyen[%i] = %li \n", i*3+1, moyen[i*3+1]);
			printf ("moyen[%i] = %li \n", i*3+2, moyen[i*3+2]);*/
    }
    if ( nb != len  ) 
    {
	printf ("%i != %i \n", nb ,  len);
    }

    //make moyen with two arrays and replace th evalue in arrays ( all_color) 
    for ( int i = 0 ; i< k ; i++)
    {
	if ( nb_pixels[i] == 0)
	{
	    continue;

	}


	SDL_Color temp_color;
	temp_color.r = moyen[i*3] / nb_pixels[i];
	temp_color.g = moyen[i*3+1] / nb_pixels[i];
	temp_color.b = moyen[i*3+2] / nb_pixels[i];
	all_color[i] = temp_color;

    }

    // Free
    free (nb_pixels);
    free (moyen);


}




int k_moyen (char* image, int k, int nb_iteration)
{

    SDL_Surface* surface = load_image( image);

    int baseh = 1000;
    int basew = 800;
    surface = RedimensionnerImage(surface, baseh, basew);

    SDL_SaveBMP(surface , "temp_files/old_surface.bmp");

    int* pix = malloc ( sizeof(int) * k); 

    Random ( pix, surface, k);

    printf ("k = %i\n", k);
    for (int i =0 ; i< k ; i++)
    {
	printf ( "%i " , pix[i]);



    }
    printf ("\n");


    // initialize the k color chooses
    Uint32* pixels = surface->pixels;
    SDL_Color* all_color = malloc (sizeof(SDL_Color) * 5 );

    for ( int i = 0 ; i < k; i++ )
    {
	SDL_Color color;
	SDL_GetRGB ( pixels [ pix[i]], surface->format , &color.r, &color.g, &color.b );
	all_color[i] = color;
    }

    // il faut verifier qu on choisit descouleur different	
    int Row = surface-> h;
    int Col = surface-> w;
    for ( int i = 1 ; i < k; i++ )
    {
	for ( int j = 0 ; j< i; j++ )
	{
	    SDL_Color color1 = all_color[i];
	    SDL_Color color2 = all_color[j];

	    if ( distance_color( color1 , color2 )  ==  0 ) 

	    {

		int temp1 = 0;

		if (getrandom (&temp1, sizeof (int), GRND_NONBLOCK) == -1)
		    perror ("getramdom");

		if ( temp1 < 0 )
		    temp1 = temp1 *-1;

		temp1  = temp1 % (Row * Col);
		SDL_Color color_temp1;
		SDL_GetRGB ( pixels[temp1], surface->format , &color_temp1.r, &color_temp1.g, &color_temp1.b );
		all_color[i] = color_temp1;
		i--;
		break;

	    }


	}
    }

    //test
    for (int i = 0 ; i < k; i++)
    {
	SDL_Color color = all_color[i];
	printf ( "%i , %i, %i \n", color.r, color.g, color.b);

    }	

    int len = surface-> w * surface->h;
    // copy of the original surface
    SDL_Surface* res_surface = SDL_ConvertSurfaceFormat( surface, SDL_PIXELFORMAT_RGB888, 0);
    Uint32* res_pixels = res_surface-> pixels;

    // first iteration for created zone
    for (int i =0 ; i< len ; i++)
    {
	SDL_Color best_color = Choose_Color ( surface, all_color, i , k );
	res_pixels [i] = SDL_MapRGB ( res_surface->format, best_color.r, best_color.g, best_color.b );

    }


    // loop to make the result more precise


    SDL_SaveBMP(res_surface , "temp_files/first_surface.bmp");

    for ( int i = 0 ; i < nb_iteration ; i++)
    {

	// average of each zone for determine k new color
	// we change array all_color

	create_k_color ( all_color, surface, res_surface , k );


	// changes each pixel of the base image with the new colors found just above
	// it's the same of : first iteration for created zone


	for (int j =0 ; j< len ; j++)
	{
	    // choose the color of pixels
	    SDL_Color best_color = Choose_Color ( surface, all_color, j , k );
	    res_pixels [j] = SDL_MapRGB ( res_surface->format, best_color.r, best_color.g, best_color.b );
	}
	printf ("%i \n", i);
    }

    res_surface = RedimensionnerImage(res_surface, baseh, basew);

    SDL_SaveBMP(res_surface , "temp_files/new_surface.bmp");

    //Free all element


    free (pix);
    free (all_color);
    SDL_FreeSurface (res_surface);
    SDL_FreeSurface (surface);
    SDL_Quit();

    printf ("K-moyen just finished !\n");

    return 1;
}
