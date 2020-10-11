#include "display.h"

void display_image(SDL_Surface *image, SDL_Texture *texture, SDL_Renderer *renderer, SDL_Rect rectangle)
{
	texture = SDL_CreateTextureFromSurface(renderer, image);

	if(SDL_QueryTexture(texture, NULL, NULL, &rectangle.w, &rectangle.h) != 0)
	{
		exit(EXIT_FAILURE);
	}
	
	rectangle.x = (800 - rectangle.w)/2;
	rectangle.y = (600 - rectangle.h)/2;


	SDL_RenderCopy(renderer, texture, NULL, &rectangle);
	SDL_RenderPresent(renderer);
}
