

struct Main_window : public Window
{
	std::vector<Image> images;


	Main_window(std::wstring dir_path)
	{
		images.reserve(10);
		for (const auto& entry : fs::directory_iterator(dir_path))
		{
			std::filesystem::path path = entry.path();
			images.push_back(std::move(Image(path.c_str())));
		}


		init(L"main window", 800, 600, DEF_STYLE, NULL, NULL, [](HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)->LRESULT
		{
			args arg = arguments.get(hwnd);
			Main_window* window = (Main_window*)arg[0];
			if (!window) return DefWindowProc(hwnd, msg, wParam, lParam);

			switch (msg)
			{
				case WM_SIZE:
				{
					window->canvas.resize(hwnd);
				}break;
				case WM_PAINT:
				{
					PAINTSTRUCT plug;
					BeginPaint(hwnd, &plug);

					draw_filled_rect(window->canvas, 0, 0, window->canvas.width, window->canvas.height, Color(255, 255, 255));

					for (int i = 0; i < window->images.size(); i++)
					{
						int x = i % 5;
						int y = i / 5;
						draw_image(window->canvas, window->images[y * 5 + x], 0.04f + x * 0.19f, 0.8f - 0.2f * y, 0.15f, 0.15f);
					}

					window->render_canvas();
					EndPaint(hwnd, &plug);
					return 0;
				}break;
				case WM_CLOSE:
				{
					running = false;
				}break;
			}

			return DefWindowProc(hwnd, msg, wParam, lParam);
		});
	}
};