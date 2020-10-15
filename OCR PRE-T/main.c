#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "pixel.h"
#include "pre_processing.h"
#include "display.h"
#include "visuals.h"
#include "segmentation.h"

#define WIN_W 800
#define WIN_H 600

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

	if (SDL_CreateWindowAndRenderer(WIN_W,WIN_H,0, &window, &renderer) != 0)
	{
		
		printf("Failed Creating Window and Renderer\n");
		return EXIT_FAILURE;
	}

	display_image(image, texture, renderer, WIN_H, WIN_W);

	SDL_Texture *buttonstex[] = {NULL, NULL, NULL};

	display_buttons(buttonstex, renderer, WIN_H, WIN_W);

	Matrix matrix;

	SDL_bool program_running = SDL_TRUE;
	while(program_running)
	{
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_MOUSEBUTTONDOWN:
				
					printf("Clic en %dx/%dy\n", event.button.x, event.button.y);
				
					if(event.button.x < WIN_W/3 && event.button.x > 5 && event.button.y > WIN_H * 0.91)
						{
							image = grayscale(image);
							//image = contrast(image);
							display_image(image, texture, renderer, WIN_H, WIN_W);
						}
				
					if(event.button.x < 2*WIN_W/3 && event.button.x > (WIN_W/3 + 5) && event.button.y > WIN_H * 0.91)
						{
							image = blackwhite(image);
							display_image(image, texture, renderer, WIN_H, WIN_W);
						}
				
					if(event.button.x < WIN_W && event.button.x > (2*WIN_W/3 + 5) && event.button.y > WIN_H * 0.91)
						{
							image = blackwhite(image);
							display_image(image, texture, renderer, WIN_H, WIN_W);
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


							Array histo = histoH(matrix);
							Pretty_print_array(histo);

							/*int InProcess = 0;
							int nbLines = 0;

							for(int i = 0; i < histo_length; i++)
							{
								if(histo[i] != 0 && InProcess == 0)
								{	
									InProcess = 1;
									nbLines++;
	
								}
		
								if(histo[i] == 0 && InProcess == 1)
									InProcess = 0;
	
							}
	
							printf("nbLines : %d\n", nbLines);

							Matrix lines[nbLines];

							printf("size of lines : %ld\n", sizeof(lines)/sizeof(lines[0]));

							for(int i = 0; i < sizeof(histo)/sizeof(histo[0]); i++)
								printf("Avant --- %d\n", histo[i]);

							printf("-------------%ld\n", sizeof(histo)/sizeof(histo[0]));
							Seg_Lines(matrix, histo, lines);

							for(int i = 0; i < sizeof(lines)/sizeof(lines[0]); i++)
							{
								Pretty_print(lines[i]);
							}*/		
						}
				
					continue;
				case SDL_QUIT:
					program_running = SDL_FALSE;
					break;
				default:
					break;
			}
		}	
	}

	SDL_DestroyTexture(buttonstex[0]);	
	SDL_DestroyTexture(buttonstex[1]);
	SDL_DestroyTexture(buttonstex[2]);

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);


	SDL_Quit();


	return EXIT_SUCCESS;
}
