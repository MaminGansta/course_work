#define MAX_THREADS 8
#include "guiAlexandrov/include.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	al_init(hInstance);

	Image japan(L"images/japan.jpg");

	Sharp_filter<Image, 3> sh;
	Sharp_filter<Image, 5> sh2;
	
	float time = get_time();

	//japan = box_filter(japan);
	//auto kernal = new_kernal(Sharp_filter<Image, 3>(), Sharp_filter<Image, 3>());

	//japan = sobel_avg(japan);
	//japan2 = sobel_avg(japan2);
	
	auto japan2 = sobel_avg(japan);
	auto japan3 = sobel_avg(sh2.apply(japan));
	auto japan4 = sobel_avg(sharp_filter3x3<Image, 5>(japan));


	output("%f\n", get_time() - time);
	
	image_window(japan);
	image_window(japan2);
	image_window(japan3);
	image_window(japan4);



	Window::wait_msg_proc();
	return 0;
}