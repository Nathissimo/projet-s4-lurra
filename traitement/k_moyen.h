#ifndef K_MOYEN_H
#define K_MOYEN_H

SDL_Surface* RedimensionnerImage(SDL_Surface* surface, int largeur, int hauteur);


SDL_Surface* load_image(const char* path);

void Random ( int* pix, SDL_Surface* surface, int k);

float distance_color (SDL_Color color1, SDL_Color color2);

SDL_Color Choose_Color (SDL_Surface* surface, SDL_Color* all_color, int i, int k );

void create_k_color (SDL_Color* all_color, SDL_Surface* original_surface, SDL_Surface* new_surface, int k);

int k_moyen (int argv, char* image, int k, int nb_iteration);



#endif
