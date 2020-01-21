
struct Window
{
	HWND handle;
	HDC hdc;

	Window(const WCHAR* class_name,
		const WCHAR* window_name,
		int width,
		int height,
		UINT style,
		LRESULT (CALLBACK *callback)(HWND, UINT, WPARAM, LPARAM),
		HINSTANCE& hInstance
	)
	{
		WNDCLASS window_class = {};
		window_class.style = CS_HREDRAW | CS_VREDRAW;
		window_class.lpszClassName = class_name;
		window_class.lpfnWndProc = callback;
		RegisterClass(&window_class);
		handle = CreateWindow(window_class.lpszClassName, window_name, style, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, hInstance, 0);
		hdc = GetDC(handle);
	}

	void process_messages()
	{

	}

	void draw(Render_State& surface)
	{
		StretchDIBits(hdc, 0, 0, surface.width, surface.height, 0, 0, surface.width, surface.height, surface.memory, &surface.bitmap_info, DIB_RGB_COLORS, SRCCOPY);
	}
};