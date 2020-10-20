#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "pixel.h"
#include "pre_processing.h"
#include "display.h"
#include "segmentation.h"

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

	int WIN_W = image -> w;
	int WIN_H = image -> h;
	if (SDL_CreateWindowAndRenderer(WIN_W, WIN_H, 0, &window, &renderer) != 0)
	{
		
		printf("Failed Creating Window and Renderer\n");
		return EXIT_FAILURE;
	}

	display_image(image, texture, renderer, WIN_H, WIN_W);

	Matrix matrix;

	Array_Array Text;

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

							program_running = SDL_FALSE;
							
							break;

						case SDLK_c:

							image = contrast(image);
							display_image(image, texture, renderer, WIN_H, WIN_W);
							
							break;

						case SDLK_n:

							image = noise_reduction(image);
							display_image(image, texture, renderer, WIN_H, WIN_W);
							
							break;

						case SDLK_g:

							image = grayscale(image);
							display_image(image, texture, renderer, WIN_H, WIN_W);
							
							break;

						case SDLK_w:

							image = blackwhite(image);
							display_image(image, texture, renderer, WIN_H, WIN_W);
							
							break;

						case SDLK_s:

							image = blackwhite(image);
							display_image(image, texture, renderer, WIN_H, WIN_W);
							matrix = binarize_image(image);

							Text = Segmentation(matrix);

							printf("size of text : %d\n", Text.size);
							/*Array histo = histoH(matrix);
							
							Matrix_Array lines = Seg_Lines(matrix, histo);

							for(int i = 0; i < lines.size; i++)
							{
								Pretty_print(lines.array_data[i]);
							}

							Array histov = histoV(lines.array_data[0]);

							float average = LetterSizeAverage(histov);

							printf("average : %.5f\n", average);

							Matrix_Array words = Seg_Words(lines.array_data[0], histov, average);

							for(int i = 0; i < words.size; i++)
							{
								Pretty_print(words.array_data[i]);
							}

							histov = histoV(words.array_data[2]);

							Matrix_Array letters = Seg_Letters(words.array_data[2], histov);

							for(int i = 0; i < letters.size; i++)
							{
								Pretty_print(letters.array_data[i]);
							}*/
							

							break;
					}
					break;
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
