
inline int my_abs(int value)
{
	uint32_t temp = value >> 31;
	value ^= temp;
	value += temp & 1;
	return value;
}


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

	fImage apply(fImage& original)
	{
		// TODO: return value optimization must be here, check this
		fImage res(original.width, original.height);
		
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
				// TODO: make SIMD optimization
				
				// rgb in windows is bgr (i dnk why)
				__m128 bgr = { 0.0f, 0.0f, 0.0f, 0.0f };
				for (int i = 0; i < size; i++)
				{
					for (int j = 0; j < size; j++)
					{
						__m128 pixel = _mm_load_ps(&original[(y - pad + i) * original.width + x - pad + j].b);
						__m128 coef = _mm_set_ps1(kernal[i][j]);
						bgr = _mm_add_ps(bgr, _mm_mul_ps(pixel, coef));
					}
				}
				res[y * res.width + x].b = max(min(bgr.m128_f32[0], 1.0f), 0.0f);
				res[y * res.width + x].g = max(min(bgr.m128_f32[1], 1.0f), 0.0f);
				res[y * res.width + x].r = max(min(bgr.m128_f32[2], 1.0f), 0.0f);
			}
		}
		

		// edges
		for (int i = 0; i < 2; i++)
		{
			for (int y = i * (original.height - pad); y < (1 - i) * pad + i * (original.height); y++)
			{
				for (int x = 0; x < original.width; x++)
				{
					__m128 bgr = { 0.0f, 0.0f, 0.0f, 0.0f };
					for (int i = 0; i < size; i++)
					{
						for (int j = 0; j < size; j++)
						{
							int core_y = abs(y - pad + i);
							int core_x = abs(x - pad + j);

							if (core_x >= original.width) core_x = original.width - (core_x - original.width) - 1;
							if (core_y >= original.height) core_y = original.height - (core_y - original.height) - 1;

							__m128 pixel = _mm_load_ps(&original[core_y * original.width + core_x].b);
							__m128 coef = _mm_set_ps1(kernal[i][j]);
							bgr = _mm_add_ps(bgr, _mm_mul_ps(pixel, coef));
						}
					}
					res[y * res.width + x].b = max(min(bgr.m128_f32[0], 1.0f), 0.0f);
					res[y * res.width + x].g = max(min(bgr.m128_f32[1], 1.0f), 0.0f);
					res[y * res.width + x].r = max(min(bgr.m128_f32[2], 1.0f), 0.0f);
				}
			}
		}

		for (int i = 0; i < 2; i++)
		{
			for (int y = pad; y < original.height - pad; y++)
			{
				for (int x = i * (original.width - pad); x < (1 - i) * pad + i * original.width; x++)
				{
					__m128 bgr = { 0.0f, 0.0f, 0.0f, 0.0f };
					for (int i = 0; i < size; i++)
					{
						for (int j = 0; j < size; j++)
						{
							int core_y = abs(y - pad + i);
							int core_x = abs(x - pad + j);

							if (core_x >= original.width) core_x = original.width - (core_x - original.width) - 1;
							if (core_y >= original.height) core_y = original.height - (core_y - original.height) - 1;

							__m128 pixel = _mm_load_ps(&original[core_y * original.width + core_x].b);
							__m128 coef = _mm_set_ps1(kernal[i][j]);
							bgr = _mm_add_ps(bgr, _mm_mul_ps(pixel, coef));
						}
					}
					res[y * res.width + x].b = max(min(bgr.m128_f32[0], 1.0f), 0.0f);
					res[y * res.width + x].g = max(min(bgr.m128_f32[1], 1.0f), 0.0f);
					res[y * res.width + x].r = max(min(bgr.m128_f32[2], 1.0f), 0.0f);
				}
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
