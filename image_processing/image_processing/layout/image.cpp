

struct Image
{
	int height, width;
	Color* data = NULL;
	bool invalid;

	Image(const char* filename) : invalid(false) , data(NULL)
	{
		int chanels;
		uint8_t* raw = stbi_load(filename, &width, &height, &chanels, 0);

		if (raw == NULL)
		{
			invalid = true;
			return;
		}
		
		data = new Color[width * height];
		
		int j = 1;
		int size = width * height;
		for (int i = 0; i < chanels * width * height; i += chanels)
			data[size - j++] = Color(raw[i], raw[i + 1], raw[i + 2]);

		stbi_image_free(raw);
	}

	Color& get_pixel(int x, int y)
	{
		assert(((uint32_t)y < height) | ((uint32_t)x < width));
		return data[y * width + x];
	}
	
	Color& get_pixel_scaled(int x, int y, int screen_w, int screen_h)
	{
		y = y * height / screen_h;
		x = x * width / screen_w;
	
		assert(((uint32_t)y < height) | ((uint32_t)x < width));
		return data[y * width + x];
	}

	~Image() { delete[] data; }
};


void draw_image(Render_State& surface, Image& image,
				int pos_x, int pos_y, int width, int height)
{
	width = min(width, surface.width - pos_x);
	height= min(height, surface.height- pos_y);

	std::future<void> res[8];

	for (int i = 0; i < workers.size; i++)
	{
		int from_x = pos_x + i * width / workers.size;
		int to_x = pos_x + (i + 1) * width / workers.size;

		res[i] = workers.add_task([from_x, to_x, pos_y, &height, &width, &surface, &image]()
		{
			for (int y = pos_y; y < height; y++)
				for (int x = from_x; x < to_x; x++)
					surface.memory[y * surface.width + x] = image.get_pixel_scaled(x, y, width, height);
		});
	}

	for (int i = 0; i < workers.size; i++)
		res[i].get();
}