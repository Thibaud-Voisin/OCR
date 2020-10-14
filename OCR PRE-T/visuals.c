#include "visuals.h"

void display_buttons(SDL_Texture *buttonstex[], SDL_Renderer *renderer, int height, int width)
{
	SDL_Rect button1;
	SDL_Rect button2;
	SDL_Rect button3;

	SDL_Surface *button1img;
	SDL_Surface *button2img;
	SDL_Surface *button3img;

	button1img = SDL_LoadBMP("gray.bmp");	
	button2img = SDL_LoadBMP("blackwhite.bmp");
	button3img = SDL_LoadBMP("binarize.bmp");

	buttonstex[0] = SDL_CreateTextureFromSurface(renderer, button1img);
	buttonstex[1] = SDL_CreateTextureFromSurface(renderer, button2img);
	buttonstex[2] = SDL_CreateTextureFromSurface(renderer, button3img);


	SDL_QueryTexture(buttonstex[0], NULL, NULL, &button1.w, &button1.h);
	SDL_QueryTexture(buttonstex[1], NULL, NULL, &button2.w, &button2.h);
	SDL_QueryTexture(buttonstex[2], NULL, NULL, &button3.w, &button3.h);


	button1.w = (width / 3) * 0.97;
	button2.w = (width / 3) * 0.97;
	button3.w = (width / 3) * 0.97;

	button1.x = 5;
	button1.y = height*0.91;

	button2.x = button1.x + button1.w + 5;
	button2.y = button1.y;

	button3.x = button2.x + button2.w + 5;
	button3.y = button2.y;

	SDL_RenderCopy(renderer, buttonstex[0], NULL, &button1);
	SDL_RenderCopy(renderer, buttonstex[1], NULL, &button2);
	SDL_RenderCopy(renderer, buttonstex[2], NULL, &button3);


	SDL_RenderPresent(renderer);


}


