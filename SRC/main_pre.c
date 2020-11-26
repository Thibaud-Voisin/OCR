#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "pixel.h"
#include "pre_processing.h"
#include "display.h"
#include "segmentation.h"


int main(int argc, char** argv)
{

    if(argc > 2)
        return 0;

    srand(time(NULL));

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
    double angle;
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
                        case SDLK_t:
                            
                            training();

                            break;

						case SDLK_s:

							blackwhite(image);
							matrix = binarize_image(image);
							Segmentation(matrix, image, texture, renderer); 
                            
                            break;

						case SDLK_r:


                            printf("Enter an angle of rotation :\n");
                            check = scanf("%lf", &angle);
                            if(check)
                                image_rotation(image, angle);
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
