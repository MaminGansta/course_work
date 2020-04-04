#define MAX_THREADS 8
#include "guiAlexandrov/include.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	al_init(hInstance);

	Image japan(L"images/japan.jpg");

	float time = get_time();

	japan = box_filter(japan);

	output("%f\n", get_time() - time);
	image_window(japan);

	Window::wait_msg_proc();
	return 0;
}