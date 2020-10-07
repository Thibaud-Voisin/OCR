#include "pixel.h"
#include "grayscale.h"
#include "display.h"

int main(void)
{
	SDL_Surface *image;
	image = SDL_LoadBMP("lac_en_montagne.bmp");
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
						case SDLK_d:
							//display_image(image);	
							continue;
						case SDLK_g:
							//image = grayscale(image);
							//display_image(image);
							continue;
						case SDLK_c:
							break
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
