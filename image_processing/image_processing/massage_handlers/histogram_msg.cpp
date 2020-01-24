
Window* hist_window_ptr;

LRESULT CALLBACK hist_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT res = 0;

	switch (uMsg)
	{
	case WM_CREATE:
	case WM_SIZE:
	{
		//RECT rect;
		//GetClientRect(hwnd, &rect);
		//main_surface_ptr->resize(rect.right - rect.left, rect.bottom - rect.top);
	} break;
	//case WM_DESTROY:
	case WM_CLOSE:
	{
			delete hist_window_ptr;
			hist_window_ptr = nullptr;
	}break;

	default:
		res = DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return res;
}

