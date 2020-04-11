#define MAX_THREADS 4
#include "guiAlexandrov/include.h"
#include "image_rate.cpp"
#include "gen_alg.cpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	al_init(hInstance);


	fImage image(L"images/low_contrast9.jpg");
	if (image.invalid) return 0;

	image_bread<fImage>::set_image((image));

	const int size = 4;
	small::array<image_bread<fImage>, size> first_generation(size, { 0u, 0u });

	auto [generation, best_id] = gen_alg(first_generation, 10);

	//image_window(generation[best_id].apply());

	Window::wait_msg_proc();
	return 0;
}