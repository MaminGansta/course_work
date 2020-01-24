
#define DEF_STYLE (WS_OVERLAPPEDWINDOW | WS_VISIBLE)

struct Window
{
	static std::vector<std::wstring> registred_classes;
	static std::unordered_map<int, HWND> handles;

	HWND handle;
	int id;
	HDC hdc;
	Canvas canvas;

	Window(
		const WCHAR* class_name,
		const WCHAR* window_name,
		int width,
		int height,
		UINT style,
		LRESULT(CALLBACK* callback)(HWND, UINT, WPARAM, LPARAM),
		HINSTANCE hInstance,
		HWND parent = NULL,
		Window** window_ptr = NULL,
		int id = 0
	)
	{
		if (window_ptr) *window_ptr = this;
		WNDCLASSEX wc;
		wc.cbSize = sizeof(wc);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = callback;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = class_name;
		//  wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
		wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

		std::wstring wClass_name(class_name);
		if (std::find(registred_classes.begin(), registred_classes.end(), wClass_name) == registred_classes.end())
		{
			if (!RegisterClassEx(&wc))
			{
				MessageBox(NULL, L"Cannot register class", L"Error", MB_OK);
				assert(false);
			}
			registred_classes.push_back(std::move(wClass_name));
		}

		handle = CreateWindow(class_name, window_name, style, CW_USEDEFAULT, CW_USEDEFAULT, width, height, parent, (HMENU)id,(HINSTANCE)hInstance, NULL);
		hdc = GetDC(handle);
	}

	~Window()
	{ 
		DestroyWindow(handle);
	}

	void render_canvas(Canvas& surface)
	{
		StretchDIBits(hdc, 0, 0, surface.width, surface.height, 0, 0, surface.width, surface.height, surface.memory, &surface.bitmap_info, DIB_RGB_COLORS, SRCCOPY);
	}

	void basic_msg_proc()
	{
		MSG msg;
		while (PeekMessage(&msg, handle, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
};

std::vector<std::wstring> Window::registred_classes = std::vector<std::wstring>();


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
		UINT style = WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_PUSHBUTTON
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
			(HMENU)id, // menu.
			(HINSTANCE)GetWindowLong(parent, GWL_HINSTANCE),
			NULL);      // Pointer not needed.
	}

	~Button() { DestroyWindow(handle); };
};