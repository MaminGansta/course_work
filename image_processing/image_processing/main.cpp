#include <Windows.h>

#include <cstdint>
#include <cstdio>

#include <vector>
#include <algorithm>

// global variables
bool running = true;


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


// unity build
#include "thread_pool.cpp"
thread_pool workers(2);

// laoyt
#include "canvas.cpp"
#include "window.cpp"
#include "image.cpp"
#include "input.cpp"
#include "timer.cpp"

// massage callback
#include "histogram_msg.cpp"
#include "main_win_msg.cpp"

// work stuff


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	Key_Input keys;
	Mouse_Input mouse;
	Timer timer(true);

	Image japan("images/japan.jpg");
	if (japan.invalid) return 2;
	image_ptr = &japan;

	// main window
	Window main_window(L"main_window", L"immage processing", 800, 600, WS_OVERLAPPEDWINDOW | WS_VISIBLE, main_callback, hInstance, NULL, &main_window_ptr);
	Button button(L"histogram", main_window.getHWND(), BUTTON_HIST);

	while (running)
	{
		// processs massages
		main_process_msg(main_window, keys, mouse);

		Window::default_msg_proc();

		

		//timer
		timer.update();

		// log
		//char log[128];
		//sprintf_s(log, "%d\n", timer.FPS);
		//OutputDebugStringA(log);
	}

	return 0;
}