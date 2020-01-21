

struct Image
{
	int height, width;
	Color* data;
	bool invalid;

	Image(const char* filename) : invalid(false)
	{
		int chanels;
		uint8_t* raw = stbi_load(filename, &width, &height, &chanels, 0);

		if (raw == NULL)
		{
			invalid = true;
			return;
		}
		
		data = new Color[width * height];
		
		if (chanels == 3)
		{
			int j = 0;
			int size = width * height;
			for (int i = 0; i < 3 * width * height; i += 3)
				data[size - j++] = Color(raw[i], raw[i + 1], raw[i + 2]);

		}
		else if (chanels == 4)
		{
			int j = 0;
			int size = width * height;
			for (int i = 0; i < 4 * width * height; i += 4)
				data[size - j++] = Color(raw[i], raw[i + 1], raw[i + 2], raw[i + 3]);
		}
		else
		{
			invalid = true;
			return;
		}
		stbi_image_free(raw);
	}

	Color get_pixel(int x, int y)
	{
		assert(((uint32_t)y < height) | ((uint32_t)x < width));
		return data[y * width + x];
	}

	Color get_pixel_scaled(int x, int y, int screen_w, int screen_h)
	{
		y = y * height / screen_h;
		x = x * width / screen_w;

		assert(((uint32_t)y < height) | ((uint32_t)x < width));
		return data[y * width + x];
	}

	~Image() { delete[] data; }
};