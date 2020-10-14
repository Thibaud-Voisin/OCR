#include "display.h"

void display_image(SDL_Surface *image, SDL_Texture *texture, SDL_Renderer *renderer, int height, int width)
{
	SDL_Rect rectangle;
	texture = SDL_CreateTextureFromSurface(renderer, image);

	if(SDL_QueryTexture(texture, NULL, NULL, &rectangle.w, &rectangle.h) != 0)
	{
		exit(EXIT_FAILURE);
	}

	while(rectangle.h > (height*0.9))
	{
		rectangle.h *= 0.9;
		rectangle.w *= 0.9;
	}

	rectangle.x = width / 2 - rectangle.w / 2;
	rectangle.y = 0;


	SDL_RenderCopy(renderer, texture, NULL, &rectangle);
	SDL_RenderPresent(renderer);
}
