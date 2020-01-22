
struct Window
{
	HWND handle;
	HDC hdc;

	Window(
		const WCHAR* window_name,
		int width,
		int height,
		UINT style,
		LRESULT(CALLBACK* callback)(HWND, UINT, WPARAM, LPARAM),
		HINSTANCE& hInstance,
		HWND parent = NULL
	)
	{
		WNDCLASS window_class = {};
		window_class.style = CS_HREDRAW | CS_VREDRAW;
		window_class.lpszClassName = L"window";
		window_class.lpfnWndProc = callback;
		window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
		RegisterClass(&window_class);
		handle = CreateWindow(window_class.lpszClassName, window_name, style, CW_USEDEFAULT, CW_USEDEFAULT, width, height, parent, 0, hInstance, 0);
		hdc = GetDC(handle);
	}

	~Window() { DestroyWindow(handle); }

	void render_bitmap(Render_State& surface)
	{
		StretchDIBits(hdc, 0, 0, surface.width, surface.height, 0, 0, surface.width, surface.height, surface.memory, &surface.bitmap_info, DIB_RGB_COLORS, SRCCOPY);
	}
};

struct Button
{
	HWND handle;

	Button(
		const WCHAR* button_name,
		HWND parent,
		int id,
		int x = 10,
		int y = 10,
		int width = 100,
		int height = 20,
		UINT style = WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON
	)
	{
		handle = CreateWindow(
			L"BUTTON",  // Predefined class; Unicode assumed 
			button_name, // Button text 
			style,     // Styles 
			x,         // x position 
			y,         // y position 
			width,     // Button width
			height,    // Button height
			parent,    // Parent window
			NULL, // menu.
			(HINSTANCE)GetWindowLong(parent, GWL_HINSTANCE),
			NULL);      // Pointer not needed.
	}

	~Button() { DestroyWindow(handle); };
};