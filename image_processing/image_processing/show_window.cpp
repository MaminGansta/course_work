struct Show_window : Window
{
	Image image;

	Button bLoad;
	Button bGo;
	Button bCloseImageWindows;

	CheckBox cEveryTime;
	CheckBox cBlackWhite;

	Label lMaxSteps;
	Text tMaxSteps;

	Text tOutput;
	
	Show_window()
	{
		init(L"gen alg", 800, 600, [](HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, Args args)->LRESULT
			{
				Show_window* window = (Show_window*)args[0];

				switch (msg)
				{
					case WM_COMMAND:
					{
						if (LOWORD(wParam) == window->bLoad.id)
						{
							wchar_t file_name[128];
							wchar_t buff[32];

							OPENFILENAME ofn;
							ZeroMemory(&ofn, sizeof(OPENFILENAME));

							ofn.lStructSize = sizeof(OPENFILENAME);
							ofn.hwndOwner = window->getHWND();
							ofn.lpstrFile = file_name;
							file_name[0] = L'\0';
							ofn.nMaxFile = 128;
							ofn.nFilterIndex = 1;

							GetOpenFileName(&ofn);
							if (wcsnlen_s(file_name, 128) == 0)	break;

							window->image.open(file_name);

							if (window->cBlackWhite.checked())
								window->image = RGB2BW(window->image);

							if (window->image.invalid)
							{
								MessageBox(NULL, L"Не получается открыть фото", L"", MB_OK);
								break;
							}
							window->redraw();
						}

						if (LOWORD(wParam) == window->bGo.id)
						{
							if (window->image.invalid) break;

							image_bread<Image>::set_image(window->image);
							const int size = 4;
							int steps = wtoi(window->tMaxSteps.get_text());

							// firs generation is zeroes
							small::array<image_bread<Image>, size> first_generation(size, { 0u, 0u });

							// clear log
							gen_log.clear();

							// call gen alg
							auto [generation, best_id] = gen_alg(first_generation, steps, window->cEveryTime.checked());
							image_windows.push_back(image_window(generation[best_id].apply()));

							window->tOutput.set_text(gen_log.c_str());
						}

						if (LOWORD(wParam) == window->bCloseImageWindows.id)
						{
							for (int i = 0; i < image_windows.size(); i++)
								Window::close(image_windows[i]);

							image_windows.clear();
						}

					}break;
					case WM_PAINT:
					{
						draw_filled_rect_aync(window->canvas, 0.0f, 0.0f, 1.0f, 1.0f, Color(255));
						if (!window->image.invalid)
							draw_image_async(window->canvas, window->image, 0.55f, 0.5f, 0.4f, 0.4f);
						
						window->render_canvas();
					}break;
					case WM_CLOSE:
					{
						PostQuitMessage(0);
					}break;
				}

				return DefWindowProc(hwnd, msg, wParam, lParam);
			});

		set_min_max_size(800, 600);
		bLoad.init(getHWND(), L"Выбрать фото", 0.01f, 0.05f, 0.3f, 0.2f, RESIZABLE);
		bGo.init(getHWND(), L"Запуск", 0.01f, 0.8f, 0.3f, 0.2f, RESIZABLE);
		bCloseImageWindows.init(getHWND(), L"Закрыть окна", 0.305f, 0.8f, 0.2f, 0.2f, RESIZABLE);

		
		cEveryTime.init(getHWND(), L"Выводить лучший на каждом шагу", 0.01f, 0.3f, 0.5f, 0.1f, RESIZABLE);
		cBlackWhite.init(getHWND(), L"Перевести в черно белое", 0.01f, 0.4f, 0.5f, 0.1f, RESIZABLE);

		lMaxSteps.init(getHWND(), L"Количество шагов (рекомендую начать с 10)", 0.01f, 0.55f, 0.4f, 0.1f, RESIZABLE);
		tMaxSteps.init(getHWND(), 0.01f, 0.65f, 0.4f, 0.1f, RESIZABLE, DEF_TEXT | ES_CENTER | ES_NUMBER);
		
		// set font for all components
		for (auto& comp : components[getHWND()])
			set_font_size(comp->handle, 25);

		tOutput.init(getHWND(), 0.55f, 0.55f, 0.4f, 0.4f, RESIZABLE, DEF_TEXT | WS_VSCROLL);

	}
};