#include <Windows.h>

#include <cstdint>
#include <cstdio>

// global variables
bool running = true;


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"




// unity build
#include "render_stuff.cpp"

#include "thread_pool.cpp"
thread_pool workers(2);

#include "window.cpp"
#include "image.cpp"
#include "input.cpp"
#include "massage_handler.cpp"
#include "timer.cpp"



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{

	Render_State main_surface;
	main_surface_ptr = &main_surface;

	Window window(L"immage processing", 800, 600, WS_OVERLAPPEDWINDOW | WS_VISIBLE, main_callback, hInstance);
	window_ptr = &window;


	Image japan("images/japan.jpg");
	if (japan.invalid) return 2;
	image_ptr = &japan;

	Key_Input keys;
	Mouse_Input mouse;
	Timer timer(true);


	Button button(L"button", window.handle, 0);


	while (running)
	{

		// massage handler
		main_process_msg(window, main_surface, keys, mouse);
		

		// work space
		draw_image(main_surface, japan, 0, 0, main_surface.width, main_surface.height);


		// generate WM_PAINT massage
		UpdateWindow(button.handle);

		//timer
		timer.update();

		// log
		char log[128];
		sprintf_s(log, "%d\n", timer.FPS);
		OutputDebugStringA(log);
	}

	return 0;
}