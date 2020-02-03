

template <size_t size>
struct Kernal
{
	float kernal[size][size];


	Kernal(std::initializer_list<float> coefs)
	{
		assert(size * size == coefs.size());
		auto coef_ptr = coefs.begin();
		float total = 0;

		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
			{
				total += *coef_ptr;
				kernal[i][j] = *coef_ptr++;
			}
	}

	Kernal() = default;

	Image apply(Image& original)
	{
		Image res(original.width, original.height);
		
		// main area
		int pad = size / 2;

		for (int y = 0; y < original.height; y++)
		{
			for (int x = 0; x < original.width; x++)
			{
				float r = 0, g = 0, b = 0;
				for (int i = 0; i < size; i++)
				{
					for (int j = 0; j < size; j++)
					{
						int core_y = abs(y - pad + i);
						int core_x = abs(x - pad + j);
						if (core_x >= original.width) core_x = original.width - (core_x - original.width) - 1;
						if (core_y >= original.height) core_y = original.height - (core_y - original.height) - 1;

						Color& pixel = original[core_y * original.width + core_x];
						r += pixel.r * kernal[i][j];
						g += pixel.g * kernal[i][j];
						b += pixel.b * kernal[i][j];
					}
				}
				res[y * res.width + x].r = max(min(r, 255.0f), 0);
				res[y * res.width + x].g = max(min(g, 255.0f), 0);
				res[y * res.width + x].b = max(min(b, 255.0f), 0);
			}
		}

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


// =================== Sharpness ==================

struct Sharp_filter : public Kernal<3>
{

	Sharp_filter() : Kernal({ -0.1f, -0.2f, -0.1f ,-0.2f, 2.2f, -0.2f ,-0.1f, -0.2f, -0.1f }){}
};
