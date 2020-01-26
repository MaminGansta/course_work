

Image gray_world(Image& image, Histogram hist)
{
	Image res(image.width, image.height);

	uint64_t R = 0;
	uint64_t G = 0;
	uint64_t B = 0;
	uint64_t AVG = 0;
	uint32_t count = 0;

	for (int i = 0; i < 256; i++)
	{
		count += hist.cred[i];
		R += i * hist.cred[i];
		G += i * hist.cgreen[i];
		B += i * hist.cblue[i];
	}

	AVG = (R + G + B) / 3;


	float rw = float(AVG) / R;
	float gw = float(AVG) / G;
	float bw = float(AVG) / B;


	for (int i = 0; i < image.height * image.width; i++)
	{
		res[i].r = min(image[i].r * rw, 255);
		res[i].g = min(image[i].g * gw, 255);
		res[i].b = min(image[i].b * bw, 255);
	}

	return res;
}