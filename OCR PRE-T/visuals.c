#include "visuals.h"

void display_buttons(SDL_Texture *button1tex, SDL_Renderer *renderer, int height)
{
	SDL_Rect button1;
	SDL_Surface *button1img;

	button1img = SDL_LoadBMP("gray.bmp");
	
	button1tex = SDL_CreateTextureFromSurface(renderer, button1img);

	SDL_QueryTexture(button1tex, NULL, NULL, &button1.w, &button1.h);

	button1.w = (height / 3) * 0.99;

	button1.x = 5;
	button1.y = height*0.91;

	SDL_RenderCopy(renderer, button1tex, NULL, &button1);
	SDL_RenderPresent(renderer);


}


