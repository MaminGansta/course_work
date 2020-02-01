

template <size_t size>
struct Kernal
{
	float kernal[size][size];

	Image apply(Image& original)
	{
		Image res(original.width, original.height);
		
		// main area
		int pad = size / 2;

		int x0 = pad;
		int y0 = pad;
		int width = original.width - pad;
		int height = original.height - pad;


		for (int y = y0; y < height; y++)
		{
			for (int x = x0; x < width; x++)
			{
				float r = 0, g = 0, b = 0;
				for (int i = 0; i < size; i++)
				{
					for (int j = 0; j < size; j++)
					{
						Color& pixel = original[(y - pad + i) * original.width + x - pad + j];
						r += pixel.r * kernal[i][j];
						g += pixel.g * kernal[i][j];
						b += pixel.b * kernal[i][j];
					}
				}
				res[y * res.width + x].r = r;
				res[y * res.width + x].g = g;
				res[y * res.width + x].b = b;
			}
		}

		// edges


		return res;
	}

	float* operator [] (int i) { return kernal[i]; }
};


// ====================== Gaussian filter ===========================

constexpr int mat_size(size_t sigma)
{
	return sigma * 6 + 1;
}


template <size_t sigma>
struct Gaussian_filter :  Kernal<mat_size(sigma)>
{
	using Kernal<mat_size(sigma)>::kernal;
	int size = mat_size(sigma);

	Gaussian_filter()
	{
		float A, a, c;
		float sigma_quad = sigma * sigma;
		A = 1.0f / (2.0f * PI * sigma_quad);
		a = c = 1.0f / (2.0f * sigma_quad);


		float total = 0;
		int pad = size / 2;
		for (int y = -pad; y <= pad; y++)
		{
			for (int x = -pad; x <= pad; x++)
			{
				float temp = A * expf(-(a * (x * x) + 2 * (abs(x) * abs(y)) + c * (y * y)));
				kernal[y + pad][x + pad] = temp;
				total += temp;
			}
		}

		// normalize coefs
		total = 1.0f / total;
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				kernal[i][j] *= total;
			}
		}
	}
};
