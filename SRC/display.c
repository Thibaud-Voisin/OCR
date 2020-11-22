#include "display.h"

/*function displays the given image if there is no problem converting it into a texture, which should never happen*/

void display_image(SDL_Surface *image, SDL_Texture *texture, SDL_Renderer *renderer)
{
	SDL_Rect rectangle;
	texture = SDL_CreateTextureFromSurface(renderer, image);

	if(SDL_QueryTexture(texture, NULL, NULL, &rectangle.w, &rectangle.h) != 0)
	{
		exit(EXIT_FAILURE);
	}

	rectangle.x = 0;
	rectangle.y = 0;


	SDL_RenderCopy(renderer, texture, NULL, &rectangle);
	SDL_RenderPresent(renderer);
}
