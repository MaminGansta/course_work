
Image hist_alignment(Image& image, Histogram hist)
{
	Image res(image.width, image.height);
	uint32_t cdf[256];
	uint32_t cdf_min;
	uint32_t count = 0;


	cdf[0] = hist.cbrightness[0];
	for (int i = 1; i < 256; i++)
	{
		cdf[i] = cdf[i - 1] + hist.cbrightness[i];
		count += hist.cbrightness[i];
	}

	for (int i = 0; i < 255; i++)
	{
		if (cdf[i] > 0)
		{
			cdf_min = cdf[i];
			break;
		}
	}


	for (int i = 0; i < image.height * image.width; i++)
	{
		Color pixel = image[i];

		float Y = 0.2126f * pixel.r + 0.7152f * pixel.g + 0.0722 * pixel.b;
		float U = -0.0999f * pixel.r - 0.336f * pixel.g + 0.436f * pixel.b;
		float V = 0.615f * pixel.r - 0.5586f * pixel.g - 0.0563 * pixel.b;

		// corection
		Y = round(float(cdf[int(Y)] - cdf_min) / (count - 1) * 255);

		Color new_color;
		new_color.r = color_clipf(Y + 1.2803f * V);
		new_color.g = color_clipf(Y - 0.2148f * U - 0.4805f * V);
		new_color.b = color_clipf(Y + 2.1279f * U);

		res[i] = new_color;
	}

	return res;
}