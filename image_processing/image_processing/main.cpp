#include <Windows.h>

#include <cstdint>
#include <cstdio>

// global variables
bool running = true;


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// unity build
#include "render_stuff.cpp"
Render_State surface;

#include "window.cpp"
#include "image.cpp"


LRESULT CALLBACK callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT res = 0;
	switch (uMsg)
	{
		case WM_CREATE:
		case WM_SIZE:
		{
			RECT rect;
			GetClientRect(hwnd, &rect);
			surface.width = rect.right - rect.left;
			surface.height = rect.bottom - rect.top;
	
			int size = surface.width * surface.height * sizeof(unsigned int);
	
			if (surface.memory) VirtualFree(surface.memory, 0, MEM_RELEASE);
			surface.memory = (Color*)VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	
			surface.bitmap_info.bmiHeader.biSize = sizeof(surface.bitmap_info.bmiHeader);
			surface.bitmap_info.bmiHeader.biWidth = surface.width;
			surface.bitmap_info.bmiHeader.biHeight = surface.height;
			surface.bitmap_info.bmiHeader.biPlanes = 1;
			surface.bitmap_info.bmiHeader.biBitCount = 32;
			surface.bitmap_info.bmiHeader.biCompression = BI_RGB;
		} break;
		case WM_DESTROY:
		case WM_CLOSE:
		{
			running = false;
		}break;
		default:
		{
			res = DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
		return res;
	}
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	Window window(L"main", L"immage processing", 800, 600, WS_OVERLAPPEDWINDOW | WS_VISIBLE, callback, hInstance);

	Image japane("images/japan.jpg");
	if (japane.invalid) return 2;


	while (running)
	{
		MSG msg;
		while (PeekMessage(&msg, window.handle, 0, 0, PM_REMOVE))
		{
			uint32_t message_code = msg.message;
			switch (message_code)
			{
				default:
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
		}

		for (int i = 0; i < surface.height; i++)
			for (int j = 0; j < surface.width; j++)
				surface.memory[i * surface.width + j] = japane.get_pixel_scaled(j, i, surface.width, surface.height);
		

		window.draw(surface);
	}

	return 0;
}