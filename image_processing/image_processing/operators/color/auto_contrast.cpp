
uint8_t color_clipf(float color)
{
	return MIN(MAX(color, 0), 255);
}


Image auto_contrast(Image& image, Histogram& hist)
{
	Image res(image.width, image.height);
	// amount of cutting, 5% from bounds
	int cut_amount = lrint(0.01f * image.height * image.width);
	int x_min, x_max;


	// find new bounds
	int count = 0;
	for (int i = 0; i < 256; i++)
	{
		count += hist.cbrightness[i];
		if (count >= cut_amount)
		{
			x_min = i;
			break;
		}
	}
	
	count = 0;
	for (int i = 255; i >= 0; i--)
	{
		count += hist.cbrightness[i];
		if (count >= cut_amount)
		{
			x_max = i;
			break;
		}
	}

	float c = 256.0f / (x_max - x_min);
	for (int i = 0; i < image.height * image.width; i++)
	{
		Color pixel = image[i];
		//uint8_t brightness = (pixel.r + pixel.g + pixel.b) / 3;

		//float ml = ((brightness - x_min) * c) / brightness;;
		//pixel.smart_mult(ml);

		float Y = 0.2126f * pixel.r + 0.7152f * pixel.g + 0.0722 * pixel.b;
		float U = -0.0999f * pixel.r - 0.336f * pixel.g + 0.436f * pixel.b;
		float V = 0.615f * pixel.r - 0.5586f * pixel.g - 0.0563 * pixel.b;
		
		// corection
		Y = (Y - x_min) * c;

		Color new_color;
		new_color.r = color_clipf(Y + 1.2803f * V);
		new_color.g = color_clipf(Y - 0.2148f * U - 0.4805f * V);
		new_color.b = color_clipf(Y + 2.1279f * U);

		// experiment
		//float coef = Y / brightness;
		//res[i] = new_color.smart_mult(coef);

		res[i] = new_color;

	}

	return res;
}