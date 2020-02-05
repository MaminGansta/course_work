
#define BTN_HIST 0

struct Window_Histogram
{
	Window** window;
	Histogram* hist;
};

struct Hist_window : public Window
{
	Window_Histogram argum;

	Hist_window(Window_Histogram in_argum) : argum(in_argum)
	{
		init(L"histogram", 1100, 400, DEF_STYLE, NULL, &argum, [](HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)->LRESULT
		{
			args arg = arguments.get(hwnd);
			Window* hist_window = (Window*)arg[0];
			if (!hist_window) return DefWindowProc(hwnd, msg, wParam, lParam);

			Window_Histogram* win_hist = (Window_Histogram*)arg[1];
			Window** pHist_window = win_hist->window;
			Histogram* hist = win_hist->hist;

			switch (msg)
			{
			case WM_SIZE:
			{
				hist_window->canvas.resize(hwnd);
			}break;
			case WM_PAINT:
			{
				PAINTSTRUCT plug;
				BeginPaint(hwnd, &plug);

				//draw_filled_rect(hist_window->canvas, 0, 0, 1, 1, Color(0));
				draw_histogram(hist_window->canvas, *hist, 0.02f, 0.05f);
				hist_window->render_canvas();

				EndPaint(hwnd, &plug);
			}break;
			case WM_CLOSE:
			{
				safe_release(pHist_window);
				return 0;
			}break;
			}

			return DefWindowProc(hwnd, msg, wParam, lParam);
		});
	}
};



struct Image_window : public Window
{
	Image img;
	Histogram hist;
	Window* hist_win;

	Button btn_hist;


	Image_window(Image& image) : img(image), hist(img), hist_win(NULL)
	{
		
		init(L"image", image.width, image.height, DEF_STYLE, NULL, NULL, [](HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)->LRESULT
		{

			args arg = arguments.get(hwnd);
			Image_window* window = (Image_window*)arg[0];
			if (!window) return DefWindowProc(hwnd, msg, wParam, lParam);

			switch (msg)
			{
				case WM_CREATE:
				case WM_SIZE:
				{
					window->canvas.resize(hwnd);
				}break;
				case WM_PAINT:
				{
					PAINTSTRUCT plug;
					BeginPaint(hwnd, &plug);
					draw_image(window->canvas, window->img, 0, 0, window->canvas.width, window->canvas.height);
					window->render_canvas();
					EndPaint(hwnd, &plug);
				}break;
				case WM_COMMAND:
				{
					if (LOWORD(wParam) == BTN_HIST)
					{
						if (!window->hist_win)
						{
							window->hist_win = new Hist_window({ &window->hist_win, &window->hist });
						}
					}
				}break;
				case WM_CLOSE:
				{
					safe_release(&window->hist_win);
					safe_release(&window);
				}
				default:
				{
					return DefWindowProc(hwnd, msg, wParam, lParam);
				}
			}
			
			return 0;
		});

		btn_hist.init(L"histogram", getHWND(), BTN_HIST);
	}

};

