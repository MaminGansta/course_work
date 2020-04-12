

template <typename Image_type>
int find_max_bound_pixels(const Image_type& image)
{
	// apply extra sharp filter to find all posible bound pixels
	Image_type new_image = sobel_avg(sharp_filter3x3<Image_type, 5>(hist_alignment(image)));

	int sum = 0;
	for (int i = 0; i < new_image.width * new_image.height; i++)
		sum += new_image[i].Y;

	return sum;
}

template <typename Image_type>
float  find_max_sharpnes(const Image_type& image)
{
	// apply extra sharp filter to maximize difference
	Image_type new_image = sharp_filter3x3<Image_type, 5>(image);
	
	float sum = 0.0f;
	float max_br = 0.0f;
	for (int i = 2; i < image.height; i++)
	{
		for (int j = 2; j < image.width; j++)
		{
			if (new_image[i * image.width + j].Y > max_br)
				max_br = new_image[i * image.width + j].Y;

			float temp = abs(new_image[i * image.width + j - 2].Y - new_image[i * image.width + j].Y);
			float temp2 = abs(new_image[(i - 2) * image.width + j].Y - new_image[i * image.width + j].Y);

			sum += (temp + temp2) * (temp + temp2);
		}
	}
	sum /= max_br / 2;
	sum /= image.width * image.height;
	
	return sum;
}



// const 
int max_bound_pixels = 0;
float max_sharpnes = 0.0f;

// calculate max valurs for image_rate
template <typename Image_type>
void prepare(const Image_type& image)
{
	max_bound_pixels = find_max_bound_pixels(image);
	max_sharpnes = find_max_sharpnes(image);
}



template <typename Image_type>
float image_rate(const Image_type& image)
{


	// rate contrast
	Image_type sobel_image = sobel_avg(image);

	int contrast_sum = 0;
	for (int i = 0; i < sobel_image.width * sobel_image.height; i++)
		contrast_sum += sobel_image[i].Y;

	float contrast_rate = float(contrast_sum) / (max_bound_pixels * 0.5f);
	

	// rate sharpnes
	float sh_sum = 0.0f;
	float max_br = 0.0f;
	for (int i = 2; i < image.height; i++)
	{
		for (int j = 2; j < image.width; j++)
		{
			if (image[i * image.width + j].Y > max_br)
				max_br = image[i * image.width + j].Y;
	
			float temp = abs(image[i * image.width + j - 2].Y - image[i * image.width + j].Y);
			float temp2 = abs(image[(i - 2) * image.width + j].Y - image[i * image.width + j].Y);
	
			sh_sum += (temp + temp2) * (temp + temp2);
		}
	}
	sh_sum /= image.width * image.height;
	sh_sum /= max_br / 2;
	
	float shrpnes_rate = sh_sum / (max_sharpnes * 0.4f);
	
	return shrpnes_rate * contrast_rate;
}