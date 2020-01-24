
Window* hist_window_ptr;
Histogram* histogram;

LRESULT CALLBACK hist_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT res = 0;

	switch (uMsg)
	{
	case WM_CREATE:
	case WM_SIZE:
	{
		RECT rect;
		GetClientRect(hwnd, &rect);
		hist_window_ptr->canvas.resize(rect.right - rect.left, rect.bottom - rect.top);
	} break;
	case WM_PAINT:
	{
		PAINTSTRUCT decoy;
		BeginPaint(hwnd, &decoy);

		draw_histogram(hist_window_ptr->canvas, *histogram, 10, 30);
		hist_window_ptr->render_canvas();

		EndPaint(hwnd, &decoy);
	}break;
	//case WM_DESTROY:
	case WM_CLOSE:
	{
			delete hist_window_ptr;
	}break;
	case WM_KEYDOWN:
	{
		uint32_t vk_code = (uint32_t)wParam;
		bool is_down = ((lParam & (1 << 31)) == 0);
		switch (vk_code)
		{
			case VK_ESCAPE:
			{
				SendMessageW(hwnd, WM_CLOSE, 0, 0);
			}break;
		}
	}break;
	default:
		res = DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return res;
}

