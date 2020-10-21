#include "pre_processing.h"

/*turns the given image into levels of gray, then returns it*/
SDL_Surface* grayscale(SDL_Surface *image)
{
	for (int i = 0; i < image -> w; i++)
	{
		for(int j = 0; j < image -> h; j++)
		{
			Uint8 r,g,b;
			SDL_GetRGB(get_pixel(image, i,j), image -> format, &r, &g, &b);
			float average = r*0.3 + g*0.59 + b*0.11;
			
			r = average;
			g = average;
			b = average;

			put_pixel(image, i,j,SDL_MapRGB(image -> format, r,g,b));
		}
	}
	return image;

}

/*turns the given image into black and white only*/
SDL_Surface* blackwhite(SDL_Surface *image)
{
	for (int i = 0; i < image -> w; i++)
	{
		for(int j = 0; j < image -> h; j++)
		{
			Uint8 r,g,b;

			SDL_GetRGB(get_pixel(image, i,j), image -> format, &r, &g, &b);
			
			if((r+g+b)/3 > 127) /*if the color is closer to white, it turns white, else black*/
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


/*Create a Matrix from a black and white image : black pixels are zeros, whites are ones*/
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

/*Increase the contrast of the given image : bright pixels becomes brighter, and darks darker*/
SDL_Surface* contrast(SDL_Surface *image)
{
	float alpha  = 1.3; /*coefficient of contrast*/

	for (int i = 0; i < image -> w; i++)
	{
		for(int j = 0; j < image -> h; j++)
		{
			Uint8 r,g,b;

			SDL_GetRGB(get_pixel(image, i,j), image -> format, &r, &g, &b);

			int rs, gs, bs;
		
			/*remove 127 to all rbg components, so that if they are dark, they become negative.
			  that way, multiplying by alpha will increase the positive, and reduce the negative*/

			rs = 127 + (alpha*(r-127)); 

			gs = 127 + (alpha*(g-127));

			bs = 127 + (alpha*(b-127));

			/*keeping rgb components within bounderies*/
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

/*removing small noise -> isolated pixels that are of no importance*/
SDL_Surface* noise_reduction(SDL_Surface *image)
{
	int pixels[5];

	for(int i = 0; i < image -> w; i++)
	{
		for(int j = 0; j < image -> h; j++)
		{
			/*putting into an array all the direct neighboors of the pixel*/
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


			/*sorting the array*/
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

			/*replace the current pixel by the median pixel of his neighboors*/

			put_pixel(image, i, j, pixels[2]);
		}
	}
	return image;
}
