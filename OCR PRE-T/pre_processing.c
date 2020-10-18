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
	float alpha  = 1.3;

	for (int i = 0; i < image -> w; i++)
	{
		for(int j = 0; j < image -> h; j++)
		{
			Uint8 r,g,b;

			SDL_GetRGB(get_pixel(image, i,j), image -> format, &r, &g, &b);

			int rs, gs, bs;

			rs = 127 + (alpha*(r-127));

			gs = 127 + (alpha*(g-127));

			bs = 127 + (alpha*(b-127));

			if(rs > 255)
				rs = 255;
			if(gs > 255)
				gs = 255;
			if(bs > 255)
				bs = 255;

			if(rs < 0)
				rs = 0;
			if(gs < 0)
				gs = 0;
			if(bs < 0)
				bs = 0;

			put_pixel(image, i,j,SDL_MapRGB(image -> format, rs,gs,bs));
		}
	}
	return image;
}

SDL_Surface* noise_reduction(SDL_Surface *image)
{
	int pixels[5];

	for(int i = 0; i < image -> w; i++)
	{
		for(int j = 0; j < image -> h; j++)
		{
			pixels[0] = get_pixel(image, i,j);
			
			if(i > 0)
				pixels[1] = get_pixel(image, i-1, j);
			else
				pixels[1] = get_pixel(image, i, j);

			if(j > 0)
				pixels[2] = get_pixel(image, i, j-1);
			else
				pixels[2] = get_pixel(image, i, j);

			if(i < image -> w)
				pixels[3] = get_pixel(image, i+1, j);
			else
				pixels[3] = get_pixel(image, i, j);

			if(j < image -> h)
				pixels[4] = get_pixel(image, i, j+1);
			else
				pixels[4] = get_pixel(image, i, j);



			for(int i = 0; i < 4; i++)
			{
				for(int j = 0; j < 4-i; j++)
				{
					if(pixels[j] < pixels[j+1])
					{
						int tmp = pixels[j];
						pixels[j] = pixels[j+1];
						pixels[j+1] = tmp;
					}
				}
			}
			put_pixel(image, i, j, pixels[2]);
		}
	}
	return image;
}
