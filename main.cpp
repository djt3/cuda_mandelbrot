#include <windows.h>
#include <conio.h>
#include <iostream>
#include <chrono> 

// Z n+1 = Z^2 + c

#define RESOLUTION 1080
float old_magnitudes[RESOLUTION][RESOLUTION];

// defined in kernel.cu
void generate_image(int max, double zoom, double x_target, double y_target);
float get_mandelbrot_at_point(int x, int y, int max);

void main()
{
	HWND hwnd = FindWindowA("ConsoleWindowClass", NULL);
	HDC hdc = GetDC(hwnd);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	double zoom = 1.f;
	double x_target = 0, y_target = 0;

	while (1)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}

		char input = _getch();

		if (input == 0x50) // down
			y_target += 0.25 / (zoom * zoom);

		else if (input == 0x48) // up
			y_target -= 0.25 / (zoom * zoom);

		else if (input == 0x4b) // left
			x_target -= 0.25 / (zoom * zoom);

		else if (input == 0x4d) // right
			x_target += 0.25 / (zoom * zoom);

		else if (input == '=')
			zoom *= 1.5;
		else if (input == '-')
			zoom /= 1.5;

		else
			continue;

		//auto start = std::chrono::high_resolution_clock::now();

		generate_image(RESOLUTION, zoom, x_target, y_target);

		//auto end = std::chrono::high_resolution_clock::now();
		//auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		//std::cout << duration.count() << std::endl;

		for (int x = 0; x < RESOLUTION; x++)
		{
			for (int y = 0; y < RESOLUTION; y++) {

				float magnitude = get_mandelbrot_at_point(x, y, RESOLUTION);
				if (old_magnitudes[x][y] > magnitude - 0.025f && old_magnitudes[x][y] < magnitude + 0.025f)
					continue;

				old_magnitudes[x][y] = magnitude;
				SetPixel(hdc, x, y, RGB(255 * sinf(magnitude), 255 * tanf(magnitude), 255 * magnitude));
			}
		}
	}

	ReleaseDC(hwnd, hdc);
	DeleteDC(hdc);
	system("pause");
}