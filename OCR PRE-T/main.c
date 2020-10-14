#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "pixel.h"
#include "grayscale.h"
#include "display.h"
#include "Binarize.h"
#include "visuals.h"

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

	int width;
	int height;
	SDL_GetWindowSize(window, &width, &height);

	display_image(image, texture, renderer, window);

	SDL_Texture *button1tex = NULL;

	display_buttons(button1tex, renderer, height);
	//SDL_Rect button2;
	//SDL_Rect button3;
	


	Matrix matrix;

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
						case SDLK_q:
							break;
						case SDLK_g:
							image = grayscale(image);
							display_image(image, texture, renderer, window);
							continue;
						case SDLK_w:
						case SDLK_b:
							image = blackwhite(image);
							display_image(image, texture, renderer, window);
							if(event.key.keysym.sym == SDLK_w)
								continue;
							matrix = binarize_image(image);
							printf("%d\n", matrix.nb_column);		
							printf("%d\n", matrix.nb_rows);
							for(int i = 0; i < matrix.nb_rows; i++)
							{
								for(int j = 0; j < matrix.nb_column; j++)
								{
									printf("%.0f",matrix.matrix_data[j + (i * matrix.nb_column)]);
								}
								printf("\n");
							}
							continue;
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

	SDL_DestroyTexture(button1tex);
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);


	SDL_Quit();


	return EXIT_SUCCESS;
}
