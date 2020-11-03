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
    SDL_Surface *saveimage;
	SDL_Texture *texture = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_Window *window = NULL;

	image = SDL_LoadBMP(argv[1]);
    saveimage = SDL_LoadBMP(argv[1]);

	if(image == NULL)
	{
		printf("Invalid Image\n");
		return EXIT_FAILURE;
	}

	int WIN_W = image -> w;
	int WIN_H = image -> h;

	if(SDL_CreateWindowAndRenderer(WIN_W, WIN_H, 0, &window, &renderer) != 0)
	{	
		printf("Failed Creating Window and Renderer\n");
		return EXIT_FAILURE;
	}
	Matrix matrix;
    int angle;
    int check;

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

							contrast(image);
							
							break;

						case SDLK_n:

							noise_reduction(image);
							
							break;

						case SDLK_g:

							grayscale(image);
							
							break;

						case SDLK_w:

							blackwhite(image);
							
							break;

						case SDLK_s:

							blackwhite(image);
							matrix = binarize_image(image);
							Segmentation(matrix, image, texture, renderer);
							
                            break;

						case SDLK_r:

                            SDL_BlitSurface(saveimage, NULL, image, NULL);

                            printf("Enter an angle of rotation :\n");
                            check = scanf("%d", &angle);
                            if(check)
                                image_rotation(image, (double)angle);
                            else
                                printf("Invalid angle\n");
							
							break;
					}
					break;
				case SDL_QUIT:
					program_running = SDL_FALSE;
					break;
				default:
					break;
			}
			display_image(image, texture, renderer);
		}
	}

	free(matrix.matrix_data);

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_FreeSurface(image);
    SDL_FreeSurface(saveimage);

	SDL_Quit();


	return EXIT_SUCCESS;
}
