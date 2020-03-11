#include <Windows.h>

#include <cstdint>
#include <cstdio>

#include <vector>
#include <array>
#include <algorithm>
#include <filesystem>
namespace fs = std::filesystem;

// global variables
bool running = true;
#define PI 3.14159265359f

#include <xmmintrin.h>

#include "guiAlexandrov/include.h"

// work stuff
#include "operators/image_transform.cpp"
#include "histogram.cpp"

#include "color/auto_contrast.cpp"
#include "color/gray_world.cpp"
#include "color/histogram_alignment.cpp"
#include "convolutions/convolution.cpp"
#include "median_filter.cpp"

#include "image_window.cpp"
#include "main_window.cpp"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	al_init(hInstance);

	// test wrokspace
	fImage japan(L"images/japan.jpg");
	if (japan.invalid) return 2;

	Gaussian_filter<1> gauss;
	Sharp_filter sharp;
	Median_filter<3> median;


	Histogram h_japan(japan);

	
	//Speed_test([&gauss, &japan]() { gauss.apply(japan); });
	//Speed_test([&sharp, &japan]() { sharp.apply(japan); });
	//Speed_test([&median, &japan]() { median.apply(japan); });
	//fImage test = median.apply(japan);

	//Speed_test([&japan, &h_japan]() { hist_alignment(japan, h_japan); });
	//Speed_test([&japan, &h_japan]() { gray_world(japan, h_japan); });
	//Speed_test([&japan, &h_japan]() { auto_contrast(japan, h_japan); });
	fImage test = hist_alignment(japan, h_japan);
	fImage test2 = auto_contrast(japan, h_japan);


	new Image_window(sobel(japan));
	
	new Image_window(japan);


	// static stuff
	//Main_window main_win(L"images");

	Timer timer(true);
	while (running)
	{
		// processs massages
		Window::default_msg_proc();

		// work space



		//timer
		timer.update();

	}

	return 0;
}