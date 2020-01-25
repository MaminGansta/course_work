
#include <string.h>

struct Histogram
{
	Image* image;
	float brightness[256];
	float red[256];
	float green[256];
	float blue[256];

	uint32_t cbrightness[256];
	uint32_t cred[256];
	uint32_t cgreen[256];
	uint32_t cblue[256];

	Histogram(Image& image) : image(&image)
	{
		update_info();
	}

	void update_info()
	{
		memset(cbrightness, 0, 256 * sizeof(uint32_t));
		memset(cred, 0, 256 * sizeof(uint32_t));
		memset(cgreen, 0, 256 * sizeof(uint32_t));
		memset(cblue, 0, 256 * sizeof(uint32_t));

		int size = image->width * image->height;
		for (int i = 0; i < size; i++)
		{
			Color color = image->data[i];
			uint8_t br = (color.r + color.g + color.b) / 3;
			cbrightness[br]++;
			cred[color.r]++;
			cgreen[color.g]++;
			cblue[color.b]++;
		}

		for (int i = 0; i < 256; i++)
			brightness[i] = float(cbrightness[i]) / size;

		for (int i = 0; i < 256; i++)
			red[i] = float(cred[i]) / size;

		for (int i = 0; i < 256; i++)
			green[i] = float(cgreen[i]) / size;

		for (int i = 0; i < 256; i++)
			blue[i] = float(cblue[i]) / size;

	}

};

void draw_graph(Canvas& surface, float* hist, int pos_x, int pos_y, Color color)
{
	drawLine(surface, pos_x, pos_y, pos_x + 255, pos_y, Color(80));
	drawLine(surface, pos_x, pos_y, pos_x, pos_y + 255, Color(80));

	for (int i = 0; i < 255; i++)
	{
		int height = 255 * hist[i] * 40;
		drawLine(surface, pos_x + i + 1, pos_y, pos_x + i + 1, pos_y + height, color);
	}
}

void draw_histogram(Canvas& surface, Histogram& hist, int pos_x, int pos_y)
{
	// brightness
	draw_graph(surface, hist.brightness, pos_x, pos_y, Color(250));
	// red
	draw_graph(surface, hist.red, pos_x + 270, pos_y, Color(255, 50, 50));
	// green
	draw_graph(surface, hist.green, pos_x + 540, pos_y, Color(50, 255, 50));
	// blue
	draw_graph(surface, hist.blue, pos_x + 810, pos_y, Color(50, 50, 255));
}

