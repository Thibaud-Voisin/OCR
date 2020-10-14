#include "pre_processing.h"

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



Matrix binarize_image(SDL_Surface *image)
{
	int width = image -> w;
	int height = image -> h;
	Matrix matrix = Init_matrix(width, height);

	for(int i = 0; i < width; i++)
	{
		for(int j = 0; j < height; j++)
		{
			Uint8 r,g,b;
			SDL_GetRGB(get_pixel(image, i,j), image -> format, &r, &g, &b);
			if(r != 0)
				matrix.matrix_data[i + (matrix.nb_column*j)] = 1;
			else
				matrix.matrix_data[i + (matrix.nb_column*j)] = 0;
		}
	}
	return matrix;
}


SDL_Surface* contrast(SDL_Surface *image)
{
	int contrast = 75;

	for (int i = 0; i < image -> w; i++)
	{
		for(int j = 0; j < image -> h; j++)
		{
			Uint8 r,g,b;

			SDL_GetRGB(get_pixel(image, i,j), image -> format, &r, &g, &b);
		
			float factor = (259*(contrast + 255))/(255*(259-contrast));

			r = factor*(r-128)+128;
			
			if(r > 255)
				r = 255;
			if(r < 0)
				r = 0;
			g = factor*(g-128)+128;
			
			if(g > 255)
				g = 255;
			if(g < 0)
				g = 0;
			b = factor*(b-128)+128;
			
			if(b > 255)
				b = 255;
			if(b < 0)
				b = 0;

			put_pixel(image, i,j,SDL_MapRGB(image -> format, r,g,b));
		}
	}
	return image;
}
