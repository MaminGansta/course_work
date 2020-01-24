
#define DEF_STYLE (WS_OVERLAPPEDWINDOW | WS_VISIBLE)


struct HWND_constainer
{
	int gen_id = 0;
	std::vector<std::pair<int, HWND>> handles;

	int add(HWND handle)
	{
		handles.push_back(std::make_pair(gen_id, handle));
		return gen_id++;
	}

	void remove(int id)
	{
		auto handle = std::find_if(handles.begin(), handles.end(), [id](std::pair<int, HWND> in) {return in.first == id; });
		DestroyWindow(handle->second);
		handles.erase(handle);
	}

	HWND operator [](int id)
	{
		auto res = std::find_if(handles.begin(), handles.end(), [id](std::pair<int, HWND> in) {return in.first == id; });

		if (res == handles.end())
			return NULL;

		return res->second;
	}

	void proc_msg()
	{
		for (auto& handle : handles)
		{
			MSG msg;
			while (PeekMessage(&msg, handle.second, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	int size() { return handles.size(); };
};


struct Window
{
	static std::vector<std::wstring> registred_classes;
	static HWND_constainer handles;
	Window** pWindow;
	
	int class_id;
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
		if (window_ptr)
		{
			pWindow = window_ptr;
			*pWindow = this;
		}
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

		HWND handle = CreateWindow(class_name, window_name, style, CW_USEDEFAULT, CW_USEDEFAULT, width, height, parent, (HMENU)id, (HINSTANCE)hInstance, NULL);
		class_id = handles.add(handle);
		hdc = GetDC(handle);
	}

	~Window() 
	{
		handles.remove(class_id);
		if (pWindow) *pWindow = nullptr;
	}

	void render_canvas(Canvas* surface = NULL)
	{
		StretchDIBits(hdc, 0, 0, canvas.width, canvas.height, 0, 0, canvas.width, canvas.height, canvas.memory, &canvas.bitmap_info, DIB_RGB_COLORS, SRCCOPY);
	}

	HWND getHWND() { return handles[class_id]; }

	static void default_msg_proc() { handles.proc_msg(); }
};

std::vector<std::wstring> Window::registred_classes = std::vector<std::wstring>();
HWND_constainer Window::handles = HWND_constainer();


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