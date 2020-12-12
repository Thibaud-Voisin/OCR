#include "pre_processing.h"

/*turns the given image into levels of gray, then returns it*/
void grayscale(SDL_Surface *image)
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
}

/*turns the given image into black and white only*/
void blackwhite(SDL_Surface *image)
{
	for (int i = 0; i < image -> w; i++)
	{
		for(int j = 0; j < image -> h; j++)
		{
			Uint8 r,g,b;

			SDL_GetRGB(get_pixel(image, i,j), image -> format, &r, &g, &b);
			
			if((r+g+b)/3 > 150) /*if the color is closer to white, it turns white, else black, giving a margin of error*/
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
}


int GetMainColor(SDL_Surface *image)
{
	int width = image -> w;
	int height = image -> h;

	int nbwhite = 0;
	int nbblack = 0;

	for(int i = 0; i < width; i++)
	{
		for(int j = 0; j < height; j++)
		{
			Uint8 r,g,b;
			SDL_GetRGB(get_pixel(image, i,j), image -> format, &r, &g, &b);
			if(r != 0)
				nbblack++;
			else
				nbwhite++;
		}
	}
	if(nbwhite > nbblack)
		return 0;
	return 1;
}

/*Create a Matrix from a black and white image : black pixels are zeros, whites are ones*/
Matrix binarize_image(SDL_Surface *image)
{
	int width = image -> w;
	int height = image -> h;
	Matrix matrix = Init_matrix(width, height);
	
	int mainColor = GetMainColor(image);


	for(int i = 0; i < width; i++)
	{
		for(int j = 0; j < height; j++)
		{
			Uint8 r,g,b;
			SDL_GetRGB(get_pixel(image, i,j), image -> format, &r, &g, &b);
			
			if((r != 0 && mainColor == 1) || (r == 0 && mainColor == 0))
				matrix.matrix_data[i + (matrix.nb_column*j)] = 1;
			else
				matrix.matrix_data[i + (matrix.nb_column*j)] = 0;
		}
	}
	return matrix;
}

/*Increase the contrast of the given image : bright pixels becomes brighter, and darks darker*/
void contrast(SDL_Surface *image)
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
}

/*removing small noise -> isolated pixels that are of no importance*/
void noise_reduction(SDL_Surface *image)
{
	int width = image -> w;
	int height = image -> h;
	int pixels[5];

	for(int i = 0; i < width; i++)
	{
		for(int j = 0; j < height; j++)
		{
			//putting into an array all the direct neighboors of the pixel
			pixels[0] = get_pixel(image, i,j);
			
			if(i > 0)
				pixels[1] = get_pixel(image, i-1, j);
			else
				pixels[1] = get_pixel(image, i, j);

			if(j > 0)
				pixels[2] = get_pixel(image, i, j-1);
			else
				pixels[2] = get_pixel(image, i, j);

			if(i < width-1)
				pixels[3] = get_pixel(image, i+1, j);
			else
				pixels[3] = get_pixel(image, i, j);

			if(j < height-1)
				pixels[4] = get_pixel(image, i, j+1);
			else
				pixels[4] = get_pixel(image, i, j);


			//sorting the array
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

			//replace the current pixel by the median pixel of his neighboors

			put_pixel(image, i, j, pixels[2]);
            
		}
	}
}

void image_rotation(SDL_Surface *image, double angle)
{
	int width = image -> w;
	int height = image -> h;

    angle = angle * M_PI / 180;

    double sinV = sin(angle);
    double cosV = cos(angle);

	SDL_Surface *rotated = SDL_CreateRGBSurface(0, image -> w, image -> h, 32, 0, 0, 0, 0);

    SDL_BlitSurface(image, NULL, rotated, NULL);

    Uint32 color = 0;
    
    if(GetMainColor(image) != 0)
    {
        color = 16777215;
    }

	for(int i = 0; i < width; i++)
	{
		for(int j = 0; j < height; j++)
		{
            int x0 = width / 2;
            int y0 = height / 2;

            int x = cosV * (i - x0) + sinV * (j - y0) + x0;
            int y = -sinV * (i - x0) + cosV * (j - y0) + y0;

            Uint32 pixel;

            if(x >= 0 && x < width && y >= 0 && y < height)
                pixel = get_pixel(image, x, y);
            else
                pixel = color;

            put_pixel(rotated, i, j, pixel);
		}
	}

    SDL_BlitSurface(rotated, NULL, image, NULL);
    SDL_FreeSurface(rotated);
}
