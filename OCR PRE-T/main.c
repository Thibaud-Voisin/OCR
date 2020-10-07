#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "pixel.h"
#include "grayscale.h"
#include "display.h"

int main(int argc, char** argv)
{


	if(argc < 2)
	{
		printf("Missing Argument : Image path\n");
		return EXIT_FAILURE;
	}
	if(argc > 2)
	{
		printf("Too many arguments\n");
		return EXIT_FAILURE;
	}

	SDL_Surface *image;
	SDL_Texture *texture = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_Window *window = NULL;

	image = SDL_LoadBMP(argv[1]);

	if(image == NULL)
	{
		printf("Invalid Image\n");
		return EXIT_FAILURE;
	}

	if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		
		printf("Failed Init SDL\n");
		return EXIT_FAILURE;
	}

	if (SDL_CreateWindowAndRenderer(800,600,0, &window, &renderer) != 0)
	{
		
		printf("Failed Creating Window and Renderer\n");
		return EXIT_FAILURE;
	}


	SDL_Rect rectangle;
	
	texture = SDL_CreateTextureFromSurface(renderer, image);


	if(SDL_QueryTexture(texture, NULL, NULL, &rectangle.w, &rectangle.h) != 0)
	{
		
		printf("Failed QueryTexture\n");
		return EXIT_FAILURE;
	}

	rectangle.x = (800 - rectangle.w)/2;
	rectangle.y = (600 - rectangle.h)/2;

	SDL_RenderCopy(renderer, texture, NULL, &rectangle);
	SDL_RenderPresent(renderer);



	SDL_bool program_running = SDL_TRUE;
	while(program_running)
	{
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym)
					{
						case SDLK_b:
							break;
						case SDLK_g:
							image = grayscale(image);
							display_image(image, texture, renderer, rectangle);
							continue;
						case SDLK_w:
							image = blackwhite(image);
							display_image(image, texture, renderer, rectangle);
						default:
							continue;
					}
				case SDL_QUIT:
					program_running = SDL_FALSE;
					break;
				default:
					break;
			}
		}
	}

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();


	return EXIT_SUCCESS;
}
