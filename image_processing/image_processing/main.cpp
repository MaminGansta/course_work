#define MAX_THREADS 8
#include "guiAlexandrov/include.h"
#include "image_rate.cpp"
#include "gen_alg.cpp"
#include "show_window.cpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	al_init(hInstance);

	new Show_window();
	
	Window::wait_msg_proc();
	return 0;
}