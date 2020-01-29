#include <Windows.h>

#include <cstdint>
#include <cstdio>

#include <vector>
#include <algorithm>
#include <filesystem>
namespace fs = std::filesystem;

// global variables
bool running = true;


#include "guiAlexandrov/include.h"



// work stuff
#include "histogram.cpp"
#include "image_window.cpp"
#include "main_window.cpp"

#include "color/auto_contrast.cpp"
#include "color/gray_world.cpp"
#include "color/histogram_alignment.cpp"



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	al_init(hInstance);


	// test space


	Key_Input keys;
	Mouse_Input mouse;
	Timer timer(true);

	Image japan(L"images/japan.jpg");
	if (japan.invalid) return 2;

	//Window main_window(L"main", 800, 600, DEF_STYLE, NULL, NULL, [] (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)->LRESULT
	//{
	//	if (msg == WM_CLOSE) running = false;
	//	return DefWindowProc(hwnd, msg, wParam, lParam);
	//});

	Main_window main_win(L"images");


	//Image_window win(japan);


	//Image test = auto_contrast(japan, *histogram);
	//image_ptr = &test;
	//histogram = new Histogram(test);


	//Image test1 = gray_world(test, *histogram);
	//image_ptr = &test1;
	//histogram = new Histogram(test1);

	//Image test = hist_alignment(japan, *histogram);
	//image_ptr = &test;
	//histogram = new Histogram(test);


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