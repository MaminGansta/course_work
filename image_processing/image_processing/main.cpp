#define MAX_THREADS 8
#include "guiAlexandrov/include.h"
#include "image_rate.cpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	al_init(hInstance);

	fImage japan(L"images/japan2.jpg");
	//japan = japan;

	Sharp_filter<Image, 3> sh;
	Sharp_filter<Image, 5> sh2;
	
	float time = get_time();

	//japan = box_filter(japan);
	//auto kernal = new_kernal(Sharp_filter<Image, 3>(), Sharp_filter<Image, 3>());

	//japan = sobel_avg(japan);
	//japan2 = sobel_avg(japan2);

	auto japan2 = box_filter<Image, 5>(japan);
	auto japan3 = sharp_filter3x3<Image, 3>(japan);


	//auto japan4 = gauss_filter(auto_contrast(japan));
	auto japan4 = sharp_filter<fImage, 7, 2>(japan);

	//auto japan2 = sobel_avg(japan);
	//auto japan3 = sobel_avg(sh2.apply(japan));
	//auto japan4 = sobel_avg(sharp_filter3x3<Image, 5>(japan));


	output("%f\n", get_time() - time);

	preapare(japan);


	float res = image_rate(japan);


	time = get_time();

	float res2 = image_rate(japan2);

	output("%f\n", get_time() - time);

	float res3 = image_rate(japan3);

	
	image_window(japan);
	image_window(japan2);
	image_window(japan3);
	image_window(japan4);



	Window::wait_msg_proc();
	return 0;
}