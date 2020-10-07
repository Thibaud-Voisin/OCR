#include "grayscale.h"

SDL_Surface* grayscale(SDL_Surface *image)
{
	for (int i = 0; i < image -> w; i++)
	{
		for(int j = 0; j < image -> h; j++)
		{
			Uint8 r,g,b;
			float tempr, tempg, tempb;
			SDL_GetRGB(get_pixel(image, i,j), image -> format, &r, &g, &b);
			tempr = r*0.3;
			tempg = g*0.59;
			tempb = b*0.11;
			
			r = tempr + tempg + tempb;
			g = tempr + tempg + tempb;
			b = tempr + tempg + tempb;
			put_pixel(image, i,j,SDL_MapRGB(image -> format, r,g,b));
		}
	}
	return image;

}

SDL_Surface* blackwhite(SDL_Surface *image)
{
	for (int i = 0; i < image -> w; i++)
	{
		for(int j = 0; j < image -> h; j++)
		{
			Uint8 r,g,b;

			SDL_GetRGB(get_pixel(image, i,j), image -> format, &r, &g, &b);
			
			if((r+g+b)/3 > 127)
			{
				r = 255;
				g = 255;
				b = 255;
			}
			else
			{
				r = 0;
				g = 0;
				b = 0;
			}
			put_pixel(image, i,j,SDL_MapRGB(image -> format, r,g,b));
		}
	}
	return image;

}
