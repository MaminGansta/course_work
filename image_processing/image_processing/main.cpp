#include <Windows.h>

#include <cstdint>
#include <cstdio>

#include <vector>
#include <algorithm>
#include <filesystem>
namespace fs = std::filesystem;

// global variables
bool running = true;
#define PI 3.14159265359f

#include "guiAlexandrov/include.h"



// work stuff
#include "histogram.cpp"

#include "color/auto_contrast.cpp"
#include "color/gray_world.cpp"
#include "color/histogram_alignment.cpp"
#include "convolutions/convolution.cpp"

#include "image_window.cpp"
#include "main_window.cpp"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	al_init(hInstance);


	Image japan(L"images/japan.jpg");
	if (japan.invalid) return 2;
	
	Gaussian_filter<1> gauss;
	Sharpness sh;

	
	Image test = sh.apply(japan);

	new Image_window(test);


	
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