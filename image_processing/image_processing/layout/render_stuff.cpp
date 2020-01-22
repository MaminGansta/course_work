
struct Color
{
	union
	{
		struct { uint8_t b, g, r, a; };
		uint8_t raw[4];
		uint32_t whole;
	};

	inline Color() = default;
	inline Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) : r(r), g(g), b(b), a(a) {}
	inline Color(uint8_t color) : r(color), g(color), b(color), a(255) {}


	Color operator *(float f)
	{
		return Color(r * f, g * f, b * f);
	}
};


struct Render_State {
	int height, width;
	int whole_size;
	Color* memory;

	BITMAPINFO bitmap_info;

	~Render_State() { VirtualFree(memory, 0, MEM_RELEASE); }

	void resize(int new_width, int new_height)
	{
		width = new_width;
		height = new_height;

		whole_size = width * height;
		int size = whole_size * sizeof(unsigned int);

		VirtualFree(memory, 0, MEM_RELEASE);
		memory = (Color*)VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		bitmap_info.bmiHeader.biSize = sizeof(bitmap_info.bmiHeader);
		bitmap_info.bmiHeader.biWidth = width;
		bitmap_info.bmiHeader.biHeight = height;
		bitmap_info.bmiHeader.biPlanes = 1;
		bitmap_info.bmiHeader.biBitCount = 32;
		bitmap_info.bmiHeader.biCompression = BI_RGB;
	}

	Color& operator [] (int inx)
	{
		assert((uint32_t)inx < whole_size);
		return memory[inx];
	}

};