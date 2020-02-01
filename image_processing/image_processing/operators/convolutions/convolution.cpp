

template <size_t size>
struct Core
{
	float coef;
	float raw[size][size];


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
						r += pixel.r * raw[i][j];
						g += pixel.g * raw[i][j];
						b += pixel.b * raw[i][j];
					}
				}
				res[y * res.width + x].r = coef * r;
				res[y * res.width + x].g = coef * g;
				res[y * res.width + x].b = coef * b;
			}
		}

		// edges


		return res;
	}
};